#ifndef SAFE_QUEUE_HXX
#define SAFE_QUEUE_HXX

#include "safeQueue.hpp"

template <typename T>
void SafeQueue<T>::pushBack(T &a_item)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push_back(a_item);
    m_sem.release();
}

template <typename T>
void SafeQueue<T>::pushBack(T &&a_item)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push_back(std::move(a_item));
    m_sem.release();
}

template <typename T>
void SafeQueue<T>::pushFront(T &a_item)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push_front(a_item);
    m_sem.release();
}

template <typename T>
void SafeQueue<T>::pushFront(T &&a_item)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push_front(std::move(a_item));
    m_sem.release();
}

template <typename T>
void SafeQueue<T>::pop(T &a_item)
{
    m_sem.acquire();
    std::lock_guard<std::mutex> lock(m_mutex);
    a_item = std::move(m_queue.front());
    m_queue.pop_front();
}

template <typename T>
void SafeQueue<T>::copyQue(std::deque<std::function<void()>> &a_queue)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (size_t i = 0; i < m_queue.size(); i++) {
        if (std::get<1>(m_queue.at(i)) == true) {
            a_queue.push_back(std::get<0>(m_queue.at(i)));
        }
    }
}

template <typename T>
bool SafeQueue<T>::isEmpty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
}

template <typename T>
size_t SafeQueue<T>::getSize() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size();
}

#endif /* SAFE_QUEUE_HXX */
