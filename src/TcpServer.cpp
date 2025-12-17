#include "TcpServer.h"

#include "IStorage.h"
#include "RLECompressor.h"
#include "RLEDecompressor.h"
#include "IClientTaskRunner.h"

#include "SocketConnection.h"
#include "SocketMenu.h"
#include "SocketOutputChannel.h"
#include "AddCommand.h"
#include "GetFileCommand.h"
#include "SearchCommand.h"
#include "DeleteCommand.h"
#include "App.h"
#include "ICommand.h"

#include <stdexcept>
#include <map>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

TcpServer::TcpServer(std::uint16_t port,
                     IStorage& storage,
                     RLECompressor& compressor,
                     RLEDecompressor& decompressor,
                     IClientTaskRunner& runner)
    : listenFd_(-1),
      port_(port),
      storage_(storage),
      compressor_(compressor),
      decompressor_(decompressor),
      runner_(runner) {}

void TcpServer::run() {
    setupListeningSocket();
    acceptLoop();
}

void TcpServer::setupListeningSocket() {
    listenFd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd_ < 0) {
        throw std::runtime_error("500 Internal Server Error");
    }

    // No SO_REUSEADDR here, to stay closer to what was taught in class.

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port_);

    if (::bind(listenFd_, reinterpret_cast<sockaddr*>(&addr),
               sizeof(addr)) < 0) {
        ::close(listenFd_);
        throw std::runtime_error("500 Internal Server Error");
    }

    if (::listen(listenFd_, SOMAXCONN) < 0) {
        ::close(listenFd_);
        throw std::runtime_error("500 Internal Server Error");
    }
}

void TcpServer::acceptLoop() {
    while (true) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);
        int clientFd = ::accept(listenFd_,
                                reinterpret_cast<sockaddr*>(&clientAddr),
                                &len);
        if (clientFd < 0) {
            // Optionally log error
            continue;
        }

        // Delegate client handling to the runner.
        // The runner decides how to run this handler (thread-per-client,
        // thread pool, etc.).
        runner_.submit(clientFd, [this](int fd) {
            this->handleClient(fd);
        });
    }
}

void TcpServer::handleClient(int clientFd) {
    try {
        SocketConnection conn(clientFd);
        SocketMenu menu(conn);
        SocketOutputChannel out(clientFd);

        // Per-client commands
        AddCommand addCmd(storage_, compressor_, out);
        GetFileCommand getCmd(storage_, decompressor_, out);
        SearchCommand searchCmd(storage_, decompressor_, out);
        DeleteCommand deleteCmd(storage_, out);

        std::map<std::string, ICommand*> commands;
        commands["post"]   = &addCmd;
        commands["get"]    = &getCmd;
        commands["search"] = &searchCmd;
        commands["delete"] = &deleteCmd;

        App app(&menu, out, commands);
        app.run();
    } catch (...) {
        // Swallow per-client errors; connection will be closed by destructors.
    }
}
