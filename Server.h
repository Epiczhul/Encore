#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "Common.h"
#include "game.h"

#define PORT 25565
#define MAX_TCP_CONNECTIONS 4

class ConnectionHandler;

class Server {
private:
    SOCKET_TYPE server_fd;
    struct sockaddr_in server_addr;
    std::vector<ConnectionHandler> connections;

public:
    Server();
    void setup();
    void loop();
    ~Server();

private:
    void handle_new_connections();
    void handle_status_connection(SOCKET_TYPE client_fd, const struct sockaddr_in& client_addr);
};

#endif