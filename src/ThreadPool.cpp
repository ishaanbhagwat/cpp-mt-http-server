#include "../include/ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads) : stop(false), active_thread_count(0) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->task_queue.empty();
                    });
                    if (this->stop && this->task_queue.empty()) {
                        return;
                    }
                    task = std::move(this->task_queue.front());
                    this->task_queue.pop();
                }

                // Increment active thread count before executing task
                active_thread_count++;
                std::cout << "Thread " << std::this_thread::get_id() << " is processing a request. Active threads: " 
                          << active_thread_count.load() << std::endl;

                task();

                // Decrement active thread count after task is done
                active_thread_count--;
                std::cout << "Thread " << std::this_thread::get_id() << " finished processing. Active threads: " 
                          << active_thread_count.load() << std::endl;
            }
        });
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker: workers){
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task){
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        task_queue.push(task);
    }
    condition.notify_one();
}