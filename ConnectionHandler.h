#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <chrono>
#include "MemStream.h"
#include "packet.h"
#include "game.h"
#include "Common.h"

class ConnectionHandler {
private:
    SOCKET_TYPE client_fd;
    game::Player player;
    std::chrono::milliseconds last_keepalive;
    static constexpr int BUFFER_SIZE = 256;
    char tcp_buffer[BUFFER_SIZE];
    bool active;

public:
    ConnectionHandler();
    void initialize(SOCKET_TYPE fd, int eid);
    void process(size_t index, std::vector<ConnectionHandler>& connections);
    void closeConnection();
    bool is_active() const { return active; }
    const game::Player& get_player() const { return player; }
};

#endif