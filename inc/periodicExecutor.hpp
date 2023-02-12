#ifndef PERIODIC_EXECUTOR_HPP
#define PERIODIC_EXECUTOR_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <chrono>
#include <atomic>
#include <condition_variable>

#include "safePriorityQueue.hpp"
#include "task.hpp"

class PeriodicExecutor
{
public:
    explicit PeriodicExecutor();

    void addTask(Task &a_task);
    void addTask(Task &&a_task);
    void shutdown();
    void pause(std::chrono::milliseconds a_milTime);
    void pause();
    void resume();
    void run();
    
private:
    void waitForSignal();

private:
    std::thread m_thread;
    SafePriorityQueue<Task> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop = false;
    std::atomic<bool> m_pause = false;
};

#endif /* PERIODIC_EXECUTOR_HPP */
