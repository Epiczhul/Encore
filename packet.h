#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <string>
#include <vector>
#include "MemStream.h"
#include "game.h"

namespace packet {
    namespace util {
        typedef uint8_t uuid[16];

        int readVarInt(Stream& stream);
        int varIntSize(unsigned int n);
        void writeVarInt(Stream& stream, const int& value);
        void writeBoolean(Stream& stream, const bool& value);
        void writeString(Stream& stream, const std::string& value);
        std::string readString(Stream& stream);

        template<typename type>
        void write(Stream& stream, const type& value);

        template<typename type>
        type read(Stream& stream);
    }

    extern const uint8_t defaultDimensionNBT[300];
    extern const util::uuid playerId;
    extern const std::string welcomeMessage;
    extern const std::string b64Texture;

    class Handshake {
    public:
        static constexpr uint8_t packetId = 0x00;
        int protocolVersion;
        int serverAddressLength;
        char serverAddress[255];
        unsigned short port;
        int nextState;
        Handshake(Stream& stream);
    };

    std::string rLoginStart(Stream& stream);
    void wLoginSuccess(Stream& stream, std::string& username, util::uuid& uuid);
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
    );
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
    );
    void wPosition(
        Stream& stream,
        const double x,
        const double y,
        const double z,
        const float yaw,
        const float pitch,
        const uint8_t tpId,
        const uint8_t flags
    );
    void wChunk(
        Stream& stream,
        int x,
        int y,
        uint8_t sections,
        bool fullChunk,
        bool ignoreOld
    );
    void wEmptyChunk(
        Stream& stream,
        const int x,
        const int y,
        const bool fullChunk,
        const bool ignoreOld
    );
    void rPlayerPosition(Stream& stream, game::Player& player);
    void rPlayerPositionAndRotation(Stream& stream, game::Player& player);
    void rPlayerRotation(Stream& stream, game::Player& player);
    void rPlayerMovement(Stream& stream);
    void wKeepAlive(Stream& stream);
    void wSpawnPlayer(Stream& stream, const game::Player& player);
    void wEntityPosition(Stream& stream, const game::Player& playerOld, const game::Player& playerNew);
    void wEntityPositionRotation(Stream& stream, const game::Player& playerOld, const game::Player& playerNew);
    void wEntityRotation(Stream& stream, const game::Player& player);
    void wEntityMovement(Stream& stream, const game::Player& player);
    void wChatMessage(Stream& stream, const std::string& message, const uint8_t pos);
    void wPlayerInfoAddPlayer(Stream& stream, std::initializer_list<game::Player> players);
    void wPlayerInfoAddPlayer(Stream& stream, game::Player* players, int length);
}

#endif