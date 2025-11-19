#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <memory>

class ThreadPool {
public:
    ThreadPool(size_t threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    // Enqueue a cancellable task. The provided task will receive a cancel token
    // that it must regularly check for cancellation (cancel->load()).
    std::shared_ptr<std::atomic<bool>> enqueueCancellable(std::function<void(std::shared_ptr<std::atomic<bool>>)> task);

    // Enqueue a normal task without cancellation support
    void enqueue(std::function<void()> task);

    void stop();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex qmutex;
    std::condition_variable cv;
    std::atomic<bool> stopFlag{false};
};

extern std::unique_ptr<ThreadPool> g_thread_pool;
extern std::atomic<int> g_active_tasks;

// Ensure the global thread pool exists, initializing it lazily if needed.
ThreadPool* ensureThreadPool(size_t minThreads = 2);
