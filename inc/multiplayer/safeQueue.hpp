#ifndef SAFE_QUEUE_HPP
#define SAFE_QUEUE_HPP

#include <thread>
#include <mutex>
#include <semaphore>
#include <functional>
#include <deque>

#include "semaphore.hpp"

template <typename T>
class SafeQueue
{
public:
    explicit SafeQueue() = default;
    ~SafeQueue() = default;
    void pushBack(T &a_item);
    void pushBack(T &&a_item);
    void pushFront(T &a_item);
    void pushFront(T &&a_item);
    void pop(T &a_item);
    void copyQue(std::deque<std::function<void()>> &a_queue);
    bool isEmpty() const;
    size_t getSize() const;

private:
    std::deque<T> m_queue;
    threads::Semaphore m_sem;
    mutable std::mutex m_mutex;
};

#include "./inl/safeQueue.hxx"

#endif /* SAFE_QUEUE_HPP */
