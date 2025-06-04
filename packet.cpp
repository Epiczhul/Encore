#include "packet.h"
#include <cstring>
#include <cmath>

namespace packet {
    const uint8_t defaultDimensionNBT[300] = {
        0x0A, 0x00, 0x0F, 0x44, 0x69, 0x6D, 0x65, 0x6E,
        0x73, 0x69, 0x6F, 0x6E, 0x20, 0x43, 0x6F, 0x64,
        0x65, 0x63, 0x09, 0x00, 0x09, 0x64, 0x69, 0x6D,
        0x65, 0x6E, 0x73, 0x69, 0x6F, 0x6E, 0x0A, 0x00,
        0x00, 0x00, 0x01, 0x08, 0x00, 0x04, 0x6E, 0x61,
        0x6D, 0x65, 0x00, 0x13, 0x6D, 0x69, 0x6E, 0x65,
        0x63, 0x72, 0x61, 0x66, 0x74, 0x3A, 0x6F, 0x76,
        0x65, 0x72, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x01,
        0x00, 0x07, 0x6E, 0x61, 0x74, 0x75, 0x72, 0x61,
        0x6C, 0x01, 0x05, 0x00, 0x0D, 0x61, 0x6D, 0x62,
        0x69, 0x65, 0x6E, 0x74, 0x5F, 0x6C, 0x69, 0x67,
        0x68, 0x74, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x0B, 0x68, 0x61, 0x73, 0x5F, 0x63, 0x65, 0x69,
        0x6C, 0x69, 0x6E, 0x67, 0x01, 0x01, 0x00, 0x0C,
        0x68, 0x61, 0x73, 0x5F, 0x73, 0x6B, 0x79, 0x6C,
        0x69, 0x67, 0x68, 0x74, 0x01, 0x04, 0x00, 0x0A,
        0x66, 0x69, 0x78, 0x65, 0x64, 0x5F, 0x74, 0x69,
        0x6D, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x17, 0x70, 0x01, 0x00, 0x06, 0x73, 0x68, 0x72,
        0x75, 0x6E, 0x6B, 0x00, 0x01, 0x00, 0x09, 0x75,
        0x6C, 0x74, 0x72, 0x61, 0x77, 0x61, 0x72, 0x6D,
        0x00, 0x01, 0x00, 0x09, 0x68, 0x61, 0x73, 0x5F,
        0x72, 0x61, 0x69, 0x64, 0x73, 0x00, 0x01, 0x00,
        0x14, 0x72, 0x65, 0x73, 0x70, 0x61, 0x77, 0x6E,
        0x5F, 0x61, 0x6E, 0x63, 0x68, 0x6F, 0x72, 0x5F,
        0x77, 0x6F, 0x72, 0x6B, 0x73, 0x00, 0x01, 0x00,
        0x09, 0x62, 0x65, 0x64, 0x5F, 0x77, 0x6F, 0x72,
        0x6B, 0x73, 0x00, 0x01, 0x00, 0x0B, 0x70, 0x69,
        0x67, 0x6C, 0x69, 0x6E, 0x5F, 0x73, 0x61, 0x66,
        0x65, 0x00, 0x03, 0x00, 0x0E, 0x6C, 0x6F, 0x67,
        0x69, 0x63, 0x61, 0x6C, 0x5F, 0x68, 0x65, 0x69,
        0x67, 0x68, 0x74, 0x00, 0x00, 0x01, 0x00, 0x08,
        0x00, 0x0A, 0x69, 0x6E, 0x66, 0x69, 0x6E, 0x69,
        0x62, 0x75, 0x72, 0x6E, 0x00, 0x1C, 0x6D, 0x69,
        0x6E, 0x65, 0x63, 0x72, 0x61, 0x66, 0x74, 0x3A,
        0x69, 0x6E, 0x66, 0x69, 0x62, 0x75, 0x72, 0x6E,
        0x2D, 0x6F, 0x76, 0x65, 0x72, 0x77, 0x6F, 0x72,
        0x6C, 0x64, 0x00, 0x00
    };

    const util::uuid playerId = {
        0x45, 0x66, 0xE6, 0x9F, 0xC9, 0x07, 0x48, 0xEE, 0x8D, 0x71, 0xD7, 0xBA, 0x5A, 0xA0, 0x0D, 0x20
    };

    const std::string welcomeMessage = "{ \"translate\": \"chat.type.announcement\", \"with\": [ \"Hello World!\" ] }";

    const std::string b64Texture = "ewogICJ0aW1lc3RhbXAiIDogMTU5Njk4NDU0MjI2MywKICAicHJvZmlsZUlkIiA6ICI0Yzg0ZGRhMGM3MDg0NTg1YjI1YTQ2ZDA0NjUxYTJkMSIsCiAgInByb2ZpbGVOYW1lIiA6ICJFcGljemh1bE1DIiwKICAidGV4dHVyZXMiIDogewogICAgIlNLSU4iIDogewogICAgICAidXJsIiA6ICJodHRwOi8vdGV4dHVyZXMubWluZWNyYWZ0Lm5ldC90ZXh0dXJlLzZiYjJlNjliMzg3MGZlMmY0ZjFiYTE0YThmOWNhOGFjYzlhNzUyMGU0ZTRhOTc4NGMxOWEzYTBjOTQ0NmU0ZCIKICAgIH0sCiAgICAiQ0FQRSIgOiB7CiAgICAgICJ1cmwiIDogImh0dHA6Ly90ZXh0dXJlcy5taW5lY3JhZnQubmV0L3RleHR1cmUvNmJiMmU2OWIzODcwZmUyZjRmMWJhMTRhOGY5Y2E4YWNjOWE3NTIwZTRlNGE5Nzg0YzE5YTNhMGM5NDQ2ZTRkIgogICAgfQogIH0KfQ==";

    namespace util {
        int readVarInt(Stream& stream) {
            int numRead = 0;
            int result = 0;
            uint8_t b;
            do {
                b = stream.read();
                int value = (b & 0b01111111);
                result |= (value << (7 * numRead));
                numRead++;
                if (numRead > 5) {
                    return result;
                }
            } while ((b & 0b10000000) != 0);
            return result;
        }

        int varIntSize(unsigned int n) {
            if (n <= 0x7F) {
                return 1;
            } else if (n <= 0x3FFF) {
                return 2;
            } else if (n <= 0x1FFFF) {
                return 3;
            } else if (n <= 0xFFFFFFF) {
                return 4;
            } else {
                return 5;
            }
        }

        void writeVarInt(Stream& stream, const int& value) {
            unsigned int uvalue = (unsigned int)value;
            do {
                uint8_t temp = (uint8_t)(uvalue & 0b01111111);
                uvalue = uvalue >> 7;
                if (uvalue != 0) {
                    temp |= 0b10000000;
                }
                stream.write(temp);
            } while (uvalue != 0);
        }

        void writeBoolean(Stream& stream, const bool& value) {
            stream.write(value ? 0x01 : 0x00);
        }

        void writeString(Stream& stream, const std::string& value) {
            writeVarInt(stream, value.length());
            stream.write((uint8_t*)value.c_str(), value.length());
        }

        std::string readString(Stream& stream) {
            int size = readVarInt(stream);
            char buffer[32768]; // Max string length per Minecraft protocol
            for (int i = 0; i < size && i < 32767; ++i) {
                buffer[i] = stream.read();
            }
            buffer[size < 32768 ? size : 32767] = '\0';
            return std::string(buffer);
        }

        template<typename type>
        void write(Stream& stream, const type& value) {
            const uint8_t* ptr = (uint8_t*)&value;
            for (int i = sizeof(type) - 1; i >= 0; --i) {
                stream.write(ptr[i]);
            }
        }

        template<typename type>
        type read(Stream& stream) {
            union {
                type value;
                uint8_t bytes[sizeof(type)];
            } v;
            for (int i = sizeof(type) - 1; i >= 0; --i) {
                v.bytes[i] = stream.read();
            }
            return v.value;
        }

        template void write<double>(Stream&, const double&);
        template void write<float>(Stream&, const float&);
        template void write<int>(Stream&, const int&);
        template void write<int64_t>(Stream&, const int64_t&);
        template void write<short>(Stream&, const short&);
        template double read<double>(Stream&);
        template float read<float>(Stream&);
        template int64_t read<int64_t>(Stream&);
        template uint16_t read<uint16_t>(Stream&);
    }

    Handshake::Handshake(Stream& stream) {
        protocolVersion = util::readVarInt(stream);
        serverAddressLength = util::readVarInt(stream);
        for (int i = 0; i < serverAddressLength && i < 254; ++i) {
            serverAddress[i] = stream.read();
        }
        serverAddress[serverAddressLength < 255 ? serverAddressLength : 254] = '\0';
        port = (((short)stream.read()) << 8) | stream.read();
        nextState = util::readVarInt(stream);
    }

    std::string rLoginStart(Stream& stream) {
        return util::readString(stream);
    }

    void wLoginSuccess(Stream& stream, std::string& username, util::uuid& uuid) {
        const uint8_t size = username.length() + sizeof(util::uuid) + 2;
        stream.write(size);
        stream.write(0x02);
        util::write(stream, uuid);
        util::writeString(stream, username);
    }

    void wJoinGame(
        Stream& stream,
        int entityId,
        std::string worldName,
        Stream& nbtData,
        std::string dimensionIdentifier,
        std::string worldIdentifier,
        uint8_t maxPlayers,
        uint8_t viewDistance,
        bool reducedDebug,
        bool isDebug,
        bool enableRespawnScreen,
        bool isFlat,
        int64_t seed,
        uint8_t gamemode,
        uint8_t prevGamemode
    ) {
        int size = 22;
        size += worldName.length() + 1;
        size += nbtData.available();
        size += dimensionIdentifier.length() + 1;
        size += worldIdentifier.length() + 1;

        util::writeVarInt(stream, size);
        stream.write(0x25);
        util::write(stream, entityId);
        stream.write(gamemode);
        stream.write(prevGamemode);
        stream.write(1);
        util::writeString(stream, worldName);

        while (nbtData.available()) {
            stream.write(nbtData.read());
        }

        util::writeString(stream, dimensionIdentifier);
        util::writeString(stream, worldIdentifier);
        stream.write((uint8_t*)&seed, 8);
        stream.write(maxPlayers);
        stream.write(viewDistance);
        util::writeBoolean(stream, reducedDebug);
        util::writeBoolean(stream, enableRespawnScreen);
        util::writeBoolean(stream, isDebug);
        util::writeBoolean(stream, isFlat);
    }

    void wJoinGame(
        Stream& stream,
        const int entityId,
        const std::string worldName,
        const std::string dimensionIdentifier,
        const std::string worldIdentifier,
        const uint8_t maxPlayers,
        const uint8_t* nbtData,
        const short nbtDataLength,
        const uint8_t viewDistance,
        const bool reducedDebug,
        const bool isDebug,
        const bool enableRespawnScreen,
        const bool isFlat,
        const int64_t seed,
        const uint8_t gamemode,
        const uint8_t prevGamemode
    ) {
        int size = 22;
        size += worldName.length() + 1;
        size += nbtDataLength;
        size += dimensionIdentifier.length() + 1;
        size += worldIdentifier.length() + 1;

        util::writeVarInt(stream, size);
        stream.write(0x25);
        util::write(stream, entityId);
        stream.write(gamemode);
        stream.write(prevGamemode);
        stream.write(1);
        util::writeString(stream, worldName);
        stream.write(nbtData, nbtDataLength);
        util::writeString(stream, dimensionIdentifier);
        util::writeString(stream, worldIdentifier);
        stream.write((uint8_t*)&seed, 8);
        stream.write(maxPlayers);
        stream.write(viewDistance);
        util::writeBoolean(stream, reducedDebug);
        util::writeBoolean(stream, enableRespawnScreen);
        util::writeBoolean(stream, isDebug);
        util::writeBoolean(stream, isFlat);
    }

    void wPosition(
        Stream& stream,
        const double x,
        const double y,
        const double z,
        const float yaw,
        const float pitch,
        const uint8_t tpId,
        const uint8_t flags
    ) {
        stream.write(35);
        stream.write(0x35);
        util::write(stream, x);
        util::write(stream, y);
        util::write(stream, z);
        util::write(stream, yaw);
        util::write(stream, pitch);
        stream.write(flags);
        stream.write(tpId);
    }

    void wChunk(
        Stream& stream,
        int x,
        int y,
        uint8_t sections,
        bool fullChunk,
        bool ignoreOld
    ) {
        int size = 6493;
        util::writeVarInt(stream, size);
        stream.write(0x21);
        util::write(stream, x);
        util::write(stream, y);
        util::writeBoolean(stream, fullChunk);
        util::writeBoolean(stream, ignoreOld);
        stream.write(sections);
        // HEIGHTMAP (FLAT)
        uint8_t nbtHead[36] = {
            0x0A, 0x00, 0x0A, 0x48, 0x65, 0x69, 0x67, 0x68, 0x74, 0x6D, 0x61, 0x70, 0x73, 0x09, 0x00, 0x0F,
            0x4D, 0x4F, 0x54, 0x49, 0x4F, 0x4E, 0x5F, 0x42, 0x4C, 0x4F, 0x43, 0x4B, 0x49, 0x4E, 0x47, 0x04,
            0x00, 0x00, 0x00, 0x24
        };
        stream.write(nbtHead, 36);
        uint8_t nbtBodyPart[9] = {
            0x1F, 0x8F, 0xC7, 0xE3, 0xF1, 0xF8, 0xFC, 0x7E, 0x3F
        };
        for (int i = 0; i < 32; ++i) {
            stream.write(nbtBodyPart, 9);
        }
        stream.write(0x00); // NBT END
        // BIOMES
        for (int i = 0; i < 1024; ++i) {
            stream.write(0x00);
            stream.write(0x00);
            stream.write(0x00);
            stream.write(0x01);
        }

        int chunkSize = 2057;
        util::writeVarInt(stream, chunkSize);
        short blockCount = 4096;
        util::write(stream, blockCount);
        stream.write(4); // Bits per Block
        // PALETTE
        stream.write(3);
        stream.write(0x01); // STONE
        stream.write(0x09); // Grass
        stream.write(0x0A); // Dirt
        // CHUNK
        // LONG ARRAY SIZE
        stream.write(0b10000000);
        stream.write(0b00000010);

        for (int i = 0; i < 12 * 128; ++i) {
            stream.write(0x00);
        }
        for (int i = 0; i < 3 * 128; ++i) {
            stream.write(0x22);
        }
        for (int i = 0; i < 1 * 128; ++i) {
            stream.write(0x11);
        }

        stream.write(0x00); // EMPTY ENTITY LIST
    }

    void wEmptyChunk(
        Stream& stream,
        const int x,
        const int y,
        const bool fullChunk,
        const bool ignoreOld
    ) {
        int size = 4435;
        util::writeVarInt(stream, size);
        stream.write(0x21);
        util::write(stream, x);
        util::write(stream, y);
        util::writeBoolean(stream, fullChunk);
        util::writeBoolean(stream, ignoreOld);
        stream.write(0x00); // No section
        // HEIGHTMAP (FLAT)
        uint8_t nbtHead[36] = {
            0x0A, 0x00, 0x0A, 0x48, 0x65, 0x69, 0x67, 0x68, 0x74, 0x6D, 0x61, 0x70, 0x73, 0x09, 0x00, 0x0F,
            0x4D, 0x4F, 0x54, 0x49, 0x4F, 0x4E, 0x5F, 0x42, 0x4C, 0x4F, 0x43, 0x4B, 0x49, 0x4E, 0x47, 0x04,
            0x00, 0x00, 0x00, 0x24
        };
        stream.write(nbtHead, 36);
        uint8_t nbtBodyPart[9] = {
            0x1F, 0x8F, 0xC7, 0xE3, 0xF1, 0xF8, 0xFC, 0x7E, 0x3F
        };
        for (int i = 0; i < 32; ++i) {
            stream.write(nbtBodyPart, 9);
        }
        stream.write(0x00); // NBT END
        // BIOMES
        for (int i = 0; i < 1024; ++i) {
            stream.write(0x00);
            stream.write(0x00);
            stream.write(0x00);
            stream.write(0x01);
        }

        int chunkSize = 0;
        util::writeVarInt(stream, chunkSize);
        stream.write(0x00); // EMPTY ENTITY LIST
    }

    void rPlayerPosition(Stream& stream, game::Player& player) {
        player.posX = util::read<double>(stream);
        player.posY = util::read<double>(stream);
        player.posZ = util::read<double>(stream);
        stream.read();
    }

    void rPlayerPositionAndRotation(Stream& stream, game::Player& player) {
        player.posX = util::read<double>(stream);
        player.posY = util::read<double>(stream);
        player.posZ = util::read<double>(stream);
        player.yaw = util::read<float>(stream);
        player.pitch = util::read<float>(stream);
        stream.read();
    }

    void rPlayerRotation(Stream& stream, game::Player& player) {
        player.yaw = util::read<float>(stream);
        player.pitch = util::read<float>(stream);
        stream.read();
    }

    void rPlayerMovement(Stream& stream) {
        stream.read();
    }

    void wKeepAlive(Stream& stream) {
        stream.write(9);
        stream.write(0x20);
        int64_t i = 0x42;
        util::write(stream, i);
    }

    void wSpawnPlayer(Stream& stream, const game::Player& player) {
        stream.write(44);
        stream.write(0x04);
        stream.write(player.eid);
        util::write(stream, playerId);
        util::write(stream, player.posX);
        util::write(stream, player.posY);
        util::write(stream, player.posZ);
        stream.write(((int)std::abs(player.yaw / 360 / (1./256.))) % 256);
        stream.write(((int)std::abs(player.pitch / 360 / (1./256.))) % 256);
    }

    void wEntityPosition(Stream& stream, const game::Player& playerOld, const game::Player& playerNew) {
        stream.write(9);
        stream.write(0x28);
        stream.write(playerOld.eid);
        short dX = (playerNew.posX * 32 - playerOld.posX * 32) * 128;
        util::write(stream, dX);
        short dY = (playerNew.posY * 32 - playerOld.posY * 32) * 128;
        util::write(stream, dY);
        short dZ = (playerNew.posZ * 32 - playerOld.posZ * 32) * 128;
        util::write(stream, dZ);
        stream.write(0x00);
    }

    void wEntityPositionRotation(Stream& stream, const game::Player& playerOld, const game::Player& playerNew) {
        stream.write(11);
        stream.write(0x29);
        stream.write(playerOld.eid);
        short dX = (playerNew.posX * 32 - playerOld.posX * 32) * 128;
        util::write(stream, dX);
        short dY = (playerNew.posY * 32 - playerOld.posY * 32) * 128;
        util::write(stream, dY);
        short dZ = (playerNew.posZ * 32 - playerOld.posZ * 32) * 128;
        util::write(stream, dZ);
        stream.write(((int)std::abs(playerNew.yaw / 360 / (1./256.))) % 256);
        stream.write(((int)std::abs(playerNew.pitch / 360 / (1./256.))) % 256);
        stream.write(0x00);
    }

    void wEntityRotation(Stream& stream, const game::Player& player) {
        stream.write(5);
        stream.write(0x2A);
        stream.write(player.eid);
        stream.write(((int)std::abs(player.yaw / 360 / (1./256.))) % 256);
        stream.write(((int)std::abs(player.pitch / 360 / (1./256.))) % 256);
        stream.write(0x00);
    }

    void wEntityMovement(Stream& stream, const game::Player& player) {
        stream.write(2);
        stream.write(0x2B);
        stream.write(player.eid);
    }

    void wChatMessage(Stream& stream, const std::string& message, const uint8_t pos) {
        int size = 1 + 1 + message.length() + 1 + 16;
        util::writeVarInt(stream, size);
        stream.write(0x0E);
        util::writeString(stream, message);
        stream.write(pos);
        for (int i = 0; i < 16; ++i) {
            stream.write(0);
        }
    }

    void wPlayerInfoAddPlayer(Stream& stream, std::initializer_list<game::Player> players) {
        int size = 3;
        for (unsigned int i = 0; i < players.size(); ++i) {
            size += 16 + 1 + players.begin()[i].name.length() + 1 + 1 + 8 + 2 + b64Texture.length() + 4;
        }

        util::writeVarInt(stream, size);
        stream.write(0x33);
        stream.write(0);
        stream.write(players.size());
        for (unsigned int i = 0; i < players.size(); ++i) {
            util::write(stream, playerId);
            util::writeString(stream, players.begin()[i].name);
            stream.write(1);
            util::writeString(stream, "textures");
            util::writeVarInt(stream, b64Texture.length());
            stream.write((uint8_t*)b64Texture.c_str(), b64Texture.length());
            stream.write(0);
            stream.write(0);
            stream.write(20);
            stream.write(0x00);
        }
    }

    void wPlayerInfoAddPlayer(Stream& stream, game::Player* players, int length) {
        int size = 3;
        for (int i = 0; i < length; ++i) {
            size += 16 + 1 + players[i].name.length() + 1 + 1 + 8 + 2 + b64Texture.length() + 4;
        }

        util::writeVarInt(stream, size);
        stream.write(0x33);
        stream.write(0);
        stream.write(length);
        for (int i = 0; i < length; ++i) {
            util::write(stream, playerId);
            util::writeString(stream, players[i].name);
            stream.write(1);
            util::writeString(stream, "textures");
            util::writeVarInt(stream, b64Texture.length());
            stream.write((uint8_t*)b64Texture.c_str(), b64Texture.length());
            stream.write(0);
            stream.write(0);
            stream.write(20);
            stream.write(0x00);
        }
    }
}