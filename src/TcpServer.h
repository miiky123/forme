#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <cstdint>

class IStorage;
class RLECompressor;
class RLEDecompressor;
class IClientTaskRunner;  // new forward declaration

class TcpServer {
public:
    TcpServer(std::uint16_t port,
              IStorage& storage,
              RLECompressor& compressor,
              RLEDecompressor& decompressor,
              IClientTaskRunner& runner);

    // Blocking loop: set up listening socket and start accepting clients.
    void run();

private:
    int listenFd_;
    std::uint16_t port_;
    IStorage& storage_;
    RLECompressor& compressor_;
    RLEDecompressor& decompressor_;
    IClientTaskRunner& runner_;   // new member

    void setupListeningSocket();
    void acceptLoop();
    void handleClient(int clientFd);
};

#endif // TCP_SERVER_H
