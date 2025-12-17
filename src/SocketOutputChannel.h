#ifndef SOCKET_OUTPUT_CHANNEL_H
#define SOCKET_OUTPUT_CHANNEL_H

#include "IOutputChannel.h"
#include <string>

class SocketOutputChannel : public IOutputChannel {
public:
    explicit SocketOutputChannel(int socketFd);

    void write(const std::string& data) override;

private:
    int socketFd_;
};

#endif // SOCKET_OUTPUT_CHANNEL_H
