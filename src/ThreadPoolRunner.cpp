#include "ThreadPoolRunner.h"

ThreadPoolRunner::ThreadPoolRunner(size_t numThreads) : stop_(false) {
    // Spawn worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        workers_.emplace_back(&ThreadPoolRunner::worker, this);
    }
}

ThreadPoolRunner::~ThreadPoolRunner() {
    stop_ = true;
    condition_.notify_all(); // Wake up all threads to let them finish

    for (std::thread &worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPoolRunner::submit(int clientFd, std::function<void(int)> handler) {
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        tasks_.push({clientFd, handler});
    }
    condition_.notify_one(); // Wake up one idle thread to handle the task
}

void ThreadPoolRunner::worker() {
    while (true) {
        std::pair<int, std::function<void(int)>> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            
            // Wait until there is a task or the pool is stopping
            condition_.wait(lock, [this] {
                return stop_ || !tasks_.empty();
            });

            // Exit thread if stopping and no tasks remain
            if (stop_ && tasks_.empty()) {
                return;
            }

            // Get the next task from the queue
            task = std::move(tasks_.front());
            tasks_.pop();
        }

        // Execute the handler (client logic) outside the lock
        if (task.second) {
            task.second(task.first);
        }
    }
}