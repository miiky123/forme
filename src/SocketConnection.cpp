#include "SocketConnection.h"

#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

SocketConnection::SocketConnection(int socketFd)
    : socketFd_(socketFd) {
    if (socketFd_ < 0) {
        throw std::runtime_error("");
    }
}

SocketConnection::~SocketConnection() {
    if (socketFd_ >= 0) {
        ::close(socketFd_);
        socketFd_ = -1;
    }
}

bool SocketConnection::readLine(std::string& line) {
    line.clear();
    char ch;

    while (true) {
        ssize_t n = ::recv(socketFd_, &ch, 1, 0);
        if (n == 0) {
            // Peer closed connection
            return !line.empty(); // false if nothing read at all
        }
        if (n < 0) {
            // Error
            return false;
        }
        if (ch == '\n') {
            break;
        }
        line.push_back(ch);
    }
    return true;
}

void SocketConnection::write(const std::string& data) {
    const char* buf = data.data();
    std::size_t total = data.size();

    while (total > 0) {
        ssize_t n = ::send(socketFd_, buf, total, 0);
        if (n <= 0) {
            throw std::runtime_error("");
        }
        buf += n;
        total -= static_cast<std::size_t>(n);
    }
}
