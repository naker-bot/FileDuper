#include "thread_pool.h"
#include <iostream>
#include <algorithm>

std::unique_ptr<ThreadPool> g_thread_pool = nullptr;
std::atomic<int> g_active_tasks{0};

ThreadPool* ensureThreadPool(size_t minThreads) {
    if (!g_thread_pool) {
        size_t suggested = std::thread::hardware_concurrency();
        if (suggested == 0) suggested = minThreads;
        suggested = std::max(suggested, minThreads);
        g_thread_pool = std::make_unique<ThreadPool>(suggested);
    }
    return g_thread_pool.get();
}

ThreadPool::ThreadPool(size_t threads) {
    if (threads == 0) threads = 1;
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lk(this->qmutex);
                    this->cv.wait(lk, [this]{ return this->stopFlag.load() || !this->tasks.empty(); });
                    if (this->stopFlag.load() && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                try {
                    task();
                } catch (const std::exception &e) {
                    std::cerr << "[ThreadPool] Task threw: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "[ThreadPool] Task threw unknown exception" << std::endl;
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::stop() {
    stopFlag = true;
    cv.notify_all();
    for (auto &t : workers) if (t.joinable()) t.join();
}

std::shared_ptr<std::atomic<bool>> ThreadPool::enqueueCancellable(std::function<void(std::shared_ptr<std::atomic<bool>>)> task) {
    auto cancel = std::make_shared<std::atomic<bool>>(false);
    {
        std::lock_guard<std::mutex> lk(qmutex);
        // Wrap task and update global active counter for progress UI
        g_active_tasks.fetch_add(1);
        tasks.emplace([task, cancel]() {
            try { task(cancel); } catch (...) { }
            g_active_tasks.fetch_sub(1);
        });
    }
    cv.notify_one();
    return cancel;
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lk(qmutex);
        g_active_tasks.fetch_add(1);
        tasks.emplace([task]() {
            try { task(); } catch (...) {}
            g_active_tasks.fetch_sub(1);
        });
    }
    cv.notify_one();
}
