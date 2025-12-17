#ifndef THREAD_POOL_RUNNER_H
#define THREAD_POOL_RUNNER_H

#include "IClientTaskRunner.h"
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPoolRunner : public IClientTaskRunner {
public:
    // Constructor initializes the pool with a fixed number of worker threads
    explicit ThreadPoolRunner(size_t numThreads);

    // Destructor stops all threads gracefully
    virtual ~ThreadPoolRunner();

    // Adds a new client task to the queue
    void submit(int clientFd, std::function<void(int)> handler) override;

private:
    // The loop each worker thread executes
    void worker();

    std::vector<std::thread> workers_;
    std::queue<std::pair<int, std::function<void(int)>>> tasks_;
    
    std::mutex queueMutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
};

#endif // THREAD_POOL_RUNNER_H