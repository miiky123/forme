#ifndef ICLIENTTASKRUNNER_H
#define ICLIENTTASKRUNNER_H

#include <functional>

// An abstraction that knows how to "run" a client connection.
// TcpServer will only depend on this interface.
class IClientTaskRunner {
public:
    virtual ~IClientTaskRunner() = default;

    // Submit a new client to be handled.
    // 'handler' is the function that actually runs the client logic.
    virtual void submit(int clientFd,
                        std::function<void(int)> handler) = 0;
};

#endif // ICLIENTTASKRUNNER_H
