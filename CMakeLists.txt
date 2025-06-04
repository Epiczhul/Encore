cmake_minimum_required(VERSION 3.10)
project(MinecraftServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_SOURCES
        main.cpp
        Server.cpp
        Server.h
        ConnectionHandler.cpp
        ConnectionHandler.h
        MemStream.h
        packet.h
        game.h
        Common.h
        packet.cpp
)

set(TEST_SOURCES
        main.cpp
        Server.cpp
        Server.h
        ConnectionHandler.cpp
        ConnectionHandler.h
        MemStream.h
        packet.h
        game.h
        Common.h
        packet.cpp
)

# Main server executable
add_executable(server ${SERVER_SOURCES})

# Test executable
add_executable(tests ${TEST_SOURCES})

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Link libraries
if(WIN32)
    target_link_libraries(server ws2_32)
    target_link_libraries(tests ws2_32)
endif()

find_path(UNITY_INCLUDE_DIR unity.h)
if(UNITY_INCLUDE_DIR)
    target_include_directories(tests PRIVATE ${UNITY_INCLUDE_DIR})
else()
    message(WARNING "Unity framework not found. Test executable may not compile correctly.")
endif()

if(WIN32)
    target_compile_definitions(server PRIVATE _WIN32)
    target_compile_definitions(tests PRIVATE _WIN32)
endif()