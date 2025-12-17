#ifndef SOCKET_CONNECTION_H
#define SOCKET_CONNECTION_H

#include "IConnection.h"

class SocketConnection : public IConnection {
public:
    explicit SocketConnection(int socketFd);
    ~SocketConnection() override;

    bool readLine(std::string& line) override;
    void write(const std::string& data) override;

private:
    int socketFd_;
};

#endif // SOCKET_CONNECTION_H


