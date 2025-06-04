#ifndef GAME_H
#define GAME_H

#include <string>
#include <cstdint>

namespace game {
    struct Coordinate {
        uint8_t x;
        uint8_t y;
    };

    struct Player {
        int eid;
        std::string name;
        double posX = 0;
        double posY = 0;
        double posZ = 0;
        float yaw = 0;
        float pitch = 0;
        uint8_t chunksLoaded[8];
        uint8_t state = 0;
        int64_t lastKeepalive;

        void unloadAllChunks() {
            for (uint8_t x = 0; x < 8; ++x) {
                chunksLoaded[x] = 0;
            }
        }
        bool isChunkLoaded(uint8_t &x, uint8_t &y) {
            return 0x1 & (chunksLoaded[x] >> y);
        }
        void setChunkLoaded(uint8_t &x, uint8_t &y) {
            chunksLoaded[x] |= 0x1 << y;
        }
        Coordinate nextChunk() {
            for (uint8_t x = 0; x < 8; ++x) {
                uint8_t chunkInfo = chunksLoaded[x];
                if (chunkInfo < 0xFF) {
                    for (uint8_t y = 0; y < 8; ++y) {
                        if (!isChunkLoaded(x, y)) {
                            return { x, y };
                        }
                    }
                }
            }
            return { 255, 255 };
        }
    };
}

#endif