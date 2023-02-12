#ifndef SAF_PRIORITYE_QUEUE_HXX
#define SAF_PRIORITYE_QUEUE_HXX

#include "safePriorityQueue.hpp"

template <typename T>
void SafePriorityQueue<T>::push(T &a_item)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(a_item);
    m_sem.release();
}

template <typename T>
void SafePriorityQueue<T>::push(T &&a_item)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(a_item));
    m_sem.release();
}

template <typename T>
void SafePriorityQueue<T>::pop(T &a_item)
{
    m_sem.acquire();
    std::lock_guard<std::mutex> lock(m_mutex);
    a_item = std::move(m_queue.top());
    m_queue.pop();
}

template <typename T>
bool SafePriorityQueue<T>::isEmpty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
}

template <typename T>
size_t SafePriorityQueue<T>::getSize() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size();
}

#endif /* SAF_PRIORITYE_QUEUE_HXX */
