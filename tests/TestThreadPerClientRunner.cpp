#include <gtest/gtest.h>
#include "ThreadPerClientRunner.h"

#include <atomic>
#include <chrono>
#include <thread>

// Helper: sleep a bit to let background threads run
static void small_sleep_ms(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Test 1: submit() should call the handler in a background thread.
TEST(ThreadPerClientRunnerTest, CallsHandlerWithCorrectFd) {
    ThreadPerClientRunner runner;

    std::atomic<bool> called{false};
    std::atomic<int> observedFd{-1};

    int expectedFd = 42;

    auto handler = [&](int fd) {
        observedFd = fd;
        called = true;
    };

    runner.submit(expectedFd, handler);

    // Give the background thread some time to run
    small_sleep_ms(50);

    EXPECT_TRUE(called.load()) << "Handler was not called by ThreadPerClientRunner";
    EXPECT_EQ(expectedFd, observedFd.load()) << "Handler received wrong file descriptor";
}

// Test 2: multiple submit() calls should trigger the handler for each client.
TEST(ThreadPerClientRunnerTest, CallsHandlerForMultipleSubmits) {
    ThreadPerClientRunner runner;

    std::atomic<int> callCount{0};

    auto handler = [&](int /*fd*/) {
        callCount.fetch_add(1);
    };

    const int numClients = 5;
    for (int i = 0; i < numClients; ++i) {
        runner.submit(100 + i, handler);
    }

    // Wait a bit for all background threads to run the handler
    small_sleep_ms(100);

    EXPECT_EQ(numClients, callCount.load())
        << "Handler was not called for all submitted clients";
}
