#include "Server.h"

int main() {
    Server server;
    server.setup();
    while (true) {
        server.loop();
    }
    return 0;
}