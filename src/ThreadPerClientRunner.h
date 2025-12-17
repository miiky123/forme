#ifndef THREAD_PER_CLIENT_RUNNER_H
#define THREAD_PER_CLIENT_RUNNER_H

#include "IClientTaskRunner.h"

#include <functional>
#include <thread>

// Concrete runner that spawns a new detached thread per client.
// This matches the current assignment requirement.
class ThreadPerClientRunner : public IClientTaskRunner {
public:
    ThreadPerClientRunner() = default;

    void submit(int clientFd,
                std::function<void(int)> handler) override {
        // Spawn a new detached thread for this client
        std::thread t(std::move(handler), clientFd);
        t.detach();
    }
};

#endif // THREAD_PER_CLIENT_RUNNER_H
