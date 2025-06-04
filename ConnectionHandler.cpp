#include "ConnectionHandler.h"
#include <cstring>
#include <iostream>

ConnectionHandler::ConnectionHandler() : client_fd(INVALID_SOCKET_VALUE), active(false) {
    memset(tcp_buffer, 0, BUFFER_SIZE);
}

void ConnectionHandler::initialize(SOCKET_TYPE fd, int eid) {
    client_fd = fd;
    player.eid = eid;
    player.state = 0;
    player.name.clear();
    player.unloadAllChunks();
    last_keepalive = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    active = true;
}

void ConnectionHandler::process(size_t index, std::vector<ConnectionHandler>& connections) {
    if (!active) return;

    SocketStream client(client_fd);

    char temp_buffer[1];
    int bytes = 0;
#ifdef _WIN32
    bytes = recv(client_fd, temp_buffer, 1, MSG_PEEK);
#else
    bytes = recv(client_fd, temp_buffer, 1, MSG_PEEK);
#endif
    if (bytes == 0 || (bytes < 0 && SOCKET_ERROR_CODE != EAGAIN && SOCKET_ERROR_CODE != EWOULDBLOCK)) {
        std::cout << "Client " << index << " disconnected: " << SOCKET_ERROR_CODE << std::endl;
        closeConnection();
        return;
    }

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    if (now - last_keepalive > std::chrono::milliseconds(30000)) {
        std::cout << "Client " << index << " timed out after 30000ms" << std::endl;
        closeConnection();
        return;
    }

    if (client.available()) {
        last_keepalive = now;
        int packetLength = packet::util::readVarInt(client);
        if (packetLength < 0 || packetLength > 1024) {
            std::cout << "Invalid packet length: " << packetLength << ", closing connection " << index << std::endl;
            closeConnection();
            return;
        }
        int packetType = packet::util::readVarInt(client);
        if (packetType < 0) {
            std::cout << "Invalid packet type: " << packetType << ", closing connection " << index << std::endl;
            closeConnection();
            return;
        }

        std::cout << "Gameplay packet: type=0x" << std::hex << packetType << ", length=" << std::dec << packetLength << std::endl;

        if (packetType == 0 && player.state == 0) {
            packet::Handshake handshake(client);
            std::cout << "Gameplay Handshake: Protocol Version: " << handshake.protocolVersion
                      << ", Port: " << handshake.port << ", Next State: " << handshake.nextState << std::endl;
            player.state = handshake.nextState;
        } else if (packetType == 0 && player.state == 2) {
            std::string playerName = packet::rLoginStart(client);
            std::cout << "Login Player: " << playerName.c_str() << std::endl;
            player.name = playerName;
            packet::util::uuid uuid { 42, 42, 42, 42, 42, 42, 42, 42 };
            std::string worldIdentifier = "minecraft:overworld";

            packet::wLoginSuccess(client, playerName, uuid);
            client.flush();
            packet::wJoinGame(client, 42, worldIdentifier, worldIdentifier, worldIdentifier, 2,
                  packet::defaultDimensionNBT, 300, 8, false, false, false, false, 0, 0x00, 0x00);
            client.flush();
            packet::wPosition(client, 3 * 16, 64, 3 * 16, 0, 0, 0x42, 0x00);
            client.flush();
            player.state = 3;
        } else if (packetType == 0 && player.state == 3) {
            std::cout << "Client Spawned at tpid " << std::hex << packet::util::readVarInt(client) << std::dec << std::endl;
            player.state = 4;

            packet::wPlayerInfoAddPlayer(client, {player});
            for (size_t pid = 0; pid < connections.size(); ++pid) {
                if (pid != index && connections[pid].is_active()) {
                    SocketStream other_client(connections[pid].client_fd);
                    packet::wPlayerInfoAddPlayer(other_client, {player});
                    other_client.flush();
                }
            }

            for (size_t pid = 0; pid < connections.size(); ++pid) {
                if (pid != index && connections[pid].is_active()) {
                    packet::wSpawnPlayer(client, connections[pid].get_player());
                    SocketStream other_client(connections[pid].client_fd);
                    packet::wSpawnPlayer(other_client, player);
                }
            }

            packet::wChatMessage(client, packet::welcomeMessage, 0x01);
        } else if (packetType == 0x12 && player.state == 4) {
            game::Player p;
            packet::rPlayerPosition(client, p);
            for (size_t pid = 0; pid < connections.size(); ++pid) {
                if (pid != index && connections[pid].is_active()) {
                    SocketStream other_client(connections[pid].client_fd);
                    packet::wEntityPosition(other_client, player, p);
                }
            }
            player.posX = p.posX;
            player.posY = p.posY;
            player.posZ = p.posZ;
        } else if (packetType == 0x13 && player.state == 4) {
            game::Player p;
            packet::rPlayerPositionAndRotation(client, p);
            for (size_t pid = 0; pid < connections.size(); ++pid) {
                if (pid != index && connections[pid].is_active()) {
                    SocketStream other_client(connections[pid].client_fd);
                    packet::wEntityPositionRotation(other_client, player, p);
                }
            }
            player.posX = p.posX;
            player.posY = p.posY;
            player.posZ = p.posZ;
            player.yaw = p.yaw;
            player.pitch = p.pitch;
        } else if (packetType == 0x14 && player.state == 4) {
            packet::rPlayerRotation(client, player);
            for (size_t pid = 0; pid < connections.size(); ++pid) {
                if (pid != index && connections[pid].is_active()) {
                    SocketStream other_client(connections[pid].client_fd);
                    packet::wEntityRotation(other_client, player);
                }
            }
        } else if (packetType == 0x15 && player.state == 4) {
            packet::rPlayerMovement(client);
            for (size_t pid = 0; pid < connections.size(); ++pid) {
                if (pid != index && connections[pid].is_active()) {
                    SocketStream other_client(connections[pid].client_fd);
                    packet::wEntityMovement(other_client, player);
                }
            }
        } else {
            std::cout << "Received Packet of type " << std::hex << packetType;
            if (packetLength < 255) {
                std::cout << ":" << std::endl;
                while (packetLength > 0 && client.available()) {
                    --packetLength;
                    std::cout << std::hex << client.read() << " ";
                }
            }
            std::cout << std::dec << std::endl;
        }
    }

    if (active && player.state == 4) {
        if (now - last_keepalive > std::chrono::milliseconds(10000)) {
            std::cout << "Sending keepalive..." << std::endl;
            last_keepalive = now;
            packet::wKeepAlive(client);
            client.flush();
        }
        game::Coordinate coordinate = player.nextChunk();
        if (coordinate.x != 255) {
            std::cout << "Sending Chunk Data for Chunk (" << (int)coordinate.x << ", " << (int)coordinate.y << ")..." << std::endl;
            player.setChunkLoaded(coordinate.x, coordinate.y);
            if (coordinate.x > 2 && coordinate.x <= 6 && coordinate.y > 2 && coordinate.y <= 6) {
                packet::wChunk(client, coordinate.x, coordinate.y, 0b00001000, true, true);
            } else {
                packet::wEmptyChunk(client, coordinate.x, coordinate.y, true, true);
            }
            client.flush();
        }
    }
}

void ConnectionHandler::closeConnection() {
    if (active) {
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        client_fd = INVALID_SOCKET_VALUE;
        player.state = 0;
        player.name.clear();
        player.unloadAllChunks();
        active = false;
    }
}