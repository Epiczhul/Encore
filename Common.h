#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include "MemStream.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#endif

#ifdef _WIN32
#define SOCKET_TYPE SOCKET
#define INVALID_SOCKET_VALUE INVALID_SOCKET
#define SOCKET_ERROR_CODE WSAGetLastError()
#else
#define SOCKET_TYPE int
#define INVALID_SOCKET_VALUE -1
#define SOCKET_ERROR_CODE errno
#endif


class SocketStream : public Stream {
private:
    SOCKET_TYPE fd;
public:
    SocketStream(SOCKET_TYPE socket_fd) : fd(socket_fd) {}

    size_t write(uint8_t b) override {
#ifdef _WIN32
        return send(fd, (const char*)&b, 1, 0);
#else
        return ::write(fd, &b, 1);
#endif
    }

    size_t write(const uint8_t *buf, size_t size) override {
#ifdef _WIN32
        return send(fd, (const char*)buf, size, 0);
#else
        return ::write(fd, buf, size);
#endif
    }

    int read() override {
        uint8_t b;
#ifdef _WIN32
        ssize_t n = recv(fd, (char*)&b, 1, 0);
#else
        ssize_t n = ::read(fd, &b, 1);
#endif
        return (n == 1) ? b : -1;
    }

    int available() override {
#ifdef _WIN32
        u_long count;
        ioctlsocket(fd, FIONREAD, &count);
#else
        int count;
        ioctl(fd, FIONREAD, &count);
#endif
        return count;
    }

    void flush() override {

    }

    int peek() override {
        uint8_t b;
#ifdef _WIN32
        ssize_t n = recv(fd, (char*)&b, 1, MSG_PEEK);
#else
        ssize_t n = ::recv(fd, &b, 1, MSG_PEEK);
#endif
        return (n == 1) ? b : -1;
    }
};

#endif