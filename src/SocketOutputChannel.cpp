#include "SocketOutputChannel.h"

#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

SocketOutputChannel::SocketOutputChannel(int socketFd)
    : socketFd_(socketFd) {
    if (socketFd_ < 0) {
        throw std::runtime_error("400 Bad Request");
    }
}

void SocketOutputChannel::write(const std::string& data) {
    const char* buf = data.data();
    std::size_t left = data.size();

    while (left > 0) {
        ssize_t n = ::send(socketFd_, buf, left, 0);
        if (n <= 0) {
            throw std::runtime_error("500 Internal Server Error");
        }
        buf  += n;
        left -= static_cast<std::size_t>(n);
    }
}
