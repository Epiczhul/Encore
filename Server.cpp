#include "Server.h"
#include "ConnectionHandler.h"

Server::Server() : connections(MAX_TCP_CONNECTIONS) {
    for (auto& conn : connections) {
        conn = ConnectionHandler();
    }
}

void Server::setup() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        exit(1);
    }
#endif

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET_VALUE) {
        std::cerr << "Failed to create socket: " << SOCKET_ERROR_CODE << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        exit(1);
    }

#ifdef _WIN32
    u_long mode = 1;
    if (ioctlsocket(server_fd, FIONBIO, &mode) != 0) {
        std::cerr << "Failed to set non-blocking mode: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        exit(1);
    }
#else
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) {
        std::cerr << "Failed to set non-blocking mode: " << SOCKET_ERROR_CODE << std::endl;
        close(server_fd);
        exit(1);
    }
#endif

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Bind failed: " << SOCKET_ERROR_CODE << std::endl;
#ifdef _WIN32
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
        exit(1);
    }

    if (listen(server_fd, MAX_TCP_CONNECTIONS) == -1) {
        std::cerr << "Listen failed: " << SOCKET_ERROR_CODE << std::endl;
#ifdef _WIN32
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
        exit(1);
    }

    std::cout << "Server started, listening on port " << PORT << std::endl;
}

void Server::loop() {
    for (size_t i = 0; i < connections.size(); ++i) {
        connections[i].process(i, connections);
    }
    handle_new_connections();
#ifdef _WIN32
    Sleep(10);
#else
    usleep(10000);
#endif
}

Server::~Server() {
    for (auto& conn : connections) {
        conn.closeConnection();
    }
#ifdef _WIN32
    closesocket(server_fd);
    WSACleanup();
#else
    close(server_fd);
#endif
}

void Server::handle_new_connections() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    SOCKET_TYPE client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

    if (client_fd == INVALID_SOCKET_VALUE) {
        return;
    }

    std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << std::endl;

#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(client_fd, FIONBIO, &mode);
#else
    fcntl(client_fd, F_SETFL, O_NONBLOCK);
#endif

    uint8_t peek_buffer[32] = {0};
    int peek_bytes = 0;
    int attempts = 0;
    const int max_attempts = 300;

    while (attempts < max_attempts && peek_bytes < sizeof(peek_buffer)) {
        int bytes = 0;
#ifdef _WIN32
        bytes = recv(client_fd, (char*)(peek_buffer + peek_bytes), sizeof(peek_buffer) - peek_bytes, MSG_PEEK);
#else
        bytes = recv(client_fd, (void*)(peek_buffer + peek_bytes), sizeof(peek_buffer) - peek_bytes, MSG_PEEK);
#endif
        if (bytes > 0) {
            peek_bytes += bytes;
        } else if (bytes == 0 || (bytes < 0 && SOCKET_ERROR_CODE != EAGAIN && SOCKET_ERROR_CODE != EWOULDBLOCK)) {
            std::cout << "Connection closed or error during peek: " << SOCKET_ERROR_CODE << std::endl;
            break;
        }
#ifdef _WIN32
        Sleep(1);
#else
        usleep(1000);
#endif
        attempts++;
    }

    bool isStatus = false;
    if (peek_bytes >= 5) {
        int pos = 0;
        int packetLength = 0;
        int shift = 0;
        do {
            if (pos >= peek_bytes) break;
            uint8_t b = peek_buffer[pos++];
            packetLength |= (b & 0x7F) << shift;
            shift += 7;
            if (shift > 35 || (b & 0x80) == 0) break;
        } while (true);

        if (pos < peek_bytes && packetLength > 0 && packetLength < peek_bytes) {
            if (peek_buffer[pos++] == 0x00) {
                while (pos < peek_bytes && (peek_buffer[pos] & 0x80)) pos++;
                pos++;
                int addrLen = 0;
                shift = 0;
                do {
                    if (pos >= peek_bytes) break;
                    uint8_t b = peek_buffer[pos++];
                    addrLen |= (b & 0x7F) << shift;
                    shift += 7;
                    if (shift > 35 || (b & 0x80) == 0) break;
                } while (true);
                pos += addrLen;
                pos += 2;
                if (pos < peek_bytes) {
                    int nextState = peek_buffer[pos] & 0x7F;
                    if (nextState == 1) {
                        isStatus = true;
                        std::cout << "Detected Status connection (nextState=1)" << std::endl;
                    }
                }
            }
        }
    }

#ifdef _WIN32
    u_long mode = 0;
    ioctlsocket(client_fd, FIONBIO, &mode);
#else
    fcntl(client_fd, F_SETFL, 0);
#endif

    if (isStatus) {
        handle_status_connection(client_fd, client_addr);
        return;
    }

    for (size_t i = 0; i < connections.size(); ++i) {
        if (!connections[i].is_active()) {
            connections[i].initialize(client_fd, i);
            std::cout << "Assigned to channel " << i << std::endl;
            return;
        }
    }

    std::cout << "Too many gameplay connections" << std::endl;
#ifdef _WIN32
    closesocket(client_fd);
#else
    close(client_fd);
#endif
}

void Server::handle_status_connection(SOCKET_TYPE client_fd, const struct sockaddr_in& client_addr) {
    std::cout << "Handling Status connection from " << inet_ntoa(client_addr.sin_addr) << std::endl;
    SocketStream client(client_fd);
    int state = 0;

#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(client_fd, FIONBIO, &mode);
#else
    fcntl(client_fd, F_SETFL, O_NONBLOCK);
#endif

    int timeout_ms = 2000;
    auto start_time = std::chrono::steady_clock::now();

    while (true) {
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start_time).count();
        if (elapsed_ms > timeout_ms) {
            std::cout << "Status connection timeout" << std::endl;
            break;
        }

        if (client.available()) {
            int packetLength = packet::util::readVarInt(client);
            if (packetLength < 0 || packetLength > 1024) {
                std::cout << "Invalid packet length: " << packetLength << std::endl;
                break;
            }
            int packetType = packet::util::readVarInt(client);
            if (packetType < 0) {
                std::cout << "Invalid packet type: " << packetType << std::endl;
                break;
            }

            std::cout << "Status packet: type=0x" << std::hex << packetType << ", length=" << std::dec << packetLength << std::endl;

            if (packetType == 0 && state == 0) {
                packet::Handshake handshake(client);
                std::cout << "Status Handshake: Protocol Version: " << handshake.protocolVersion
                          << ", Port: " << handshake.port << ", Next State: " << handshake.nextState << std::endl;
                state = handshake.nextState;
                if (state != 1) {
                    std::cout << "Unexpected next state: " << state << std::endl;
                    break;
                }
            } else if (state == 1) {
                if (packetType == 0x00) {
                    int onlinePlayers = 0;
                    for (const auto& conn : connections) {
                        if (conn.is_active() && conn.get_player().state == 4) {
                            onlinePlayers++;
                        }
                    }
                    std::string statusJson = "{\"version\":{\"name\":\"Encore 1.16.1\",\"protocol\":736},\"players\":{\"max\":" +
                                             std::to_string(MAX_TCP_CONNECTIONS) + ",\"online\":" +
                                             std::to_string(onlinePlayers) + ",\"sample\":[]},\"description\":{\"text\":\"§5§lEncore §r§l- §3§lInnovation on the Block\"}}";
                    int jsonLen = statusJson.length();
                    int varIntLenSize = packet::util::varIntSize(jsonLen);
                    int totalDataLen = 1 + varIntLenSize + jsonLen;
                    packet::util::writeVarInt(client, totalDataLen);
                    client.write(0x00);
                    packet::util::writeVarInt(client, jsonLen);
                    client.write((uint8_t*)statusJson.c_str(), jsonLen);
                    client.flush();
                    std::cout << "Sent Status Response with " << onlinePlayers << "/" << MAX_TCP_CONNECTIONS << " players" << std::endl;
                } else if (packetType == 0x01) {
                    int64_t time = packet::util::read<int64_t>(client);
                    int totalDataLen = 1 + 8;
                    packet::util::writeVarInt(client, totalDataLen);
                    client.write(0x01);
                    packet::util::write(client, time);
                    client.flush();
                    std::cout << "Sent Ping Response with timestamp " << time << std::endl;
                    break;
                } else {
                    std::cout << "Unexpected packet type in Status state: 0x" << std::hex << packetType << std::dec << std::endl;
                    break;
                }
            }
        }
#ifdef _WIN32
        Sleep(1);
#else
        usleep(1000);
#endif
    }

#ifdef _WIN32
    closesocket(client_fd);
#else
    close(client_fd);
#endif
    std::cout << "Closed Status connection" << std::endl;
}