#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t num_threads); // Constructor
    ~ThreadPool();                  // Destructor

    void enqueue(std::function<void()> task); // Add a task to the pool

    // Function to get the number of active threads
    size_t active_threads() const {
        return active_thread_count.load();
    }

private:
    std::vector<std::thread> workers;                  // Worker threads
    std::queue<std::function<void()>> task_queue;      // Queue of tasks
    std::mutex queue_mutex;                            // Synchronization
    std::condition_variable condition;                 // To notify threads
    void worker_thread(); // Function each worker thread runs
    std::atomic<bool> stop;
    std::atomic<size_t> active_thread_count;  // Atomic counter for active threads
};

#endif // THREADPOOL_H
