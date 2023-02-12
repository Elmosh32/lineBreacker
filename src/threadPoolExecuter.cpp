#include "threadPoolExecuter.hpp"
#include <iostream>

ThreadPoolExecutor::ThreadPoolExecutor(size_t a_numOfThreads)
: m_numOfThreads(a_numOfThreads)
{
    CreateThreads(m_numOfThreads);
}

ThreadPoolExecutor::~ThreadPoolExecutor()
{
    RemoveAllThreads();
}

void ThreadPoolExecutor::Submit(std::function<void()> &a_task)
{
    // std::unique_lock<std::mutex> lock(m_mutex);

    if (m_running) {
        m_safeQueue.pushBack(std::make_tuple(a_task, true));
    }
}

void ThreadPoolExecutor::Submit(std::function<void()> &&a_task)
{
    // std::unique_lock<std::mutex> lock(m_mutex);

    if (m_running) {
        m_safeQueue.pushBack(std::move(std::make_tuple(a_task, true)));
    }
}

void ThreadPoolExecutor::Add(size_t a_numOfThreads)
{
    // std::unique_lock<std::mutex> lock(m_mutex);
    if (m_running) {
        m_numOfThreads += a_numOfThreads;
        CreateThreads(a_numOfThreads);
    }
}

void ThreadPoolExecutor::Remove(size_t a_numOfThreads)
{

    int counter = 0;
    if (m_running) {
        std::unique_lock<std::mutex> lock(m_mutex);

        a_numOfThreads = std::min(m_numOfThreads, a_numOfThreads);
        
        for (size_t i = 0; i < a_numOfThreads; i++) {
            m_safeQueue.pushFront(std::make_tuple(taskEnder, false));
            counter++;
        }

        Clean(a_numOfThreads);
        m_numOfThreads -= a_numOfThreads;
    }
}

void ThreadPoolExecutor::Shutdown()
{
    if (m_running) {
        StopRunning();

        for (size_t i = 0; i < m_numOfThreads; i++)
        {
            m_safeQueue.pushBack(std::make_tuple(taskEnder, false));
        }

        RemoveAllThreads();
        m_numOfThreads = 0;
    }
}

std::deque<std::function<void()>> ThreadPoolExecutor::ShutdownNow()
{
    if (m_running)
    {
        StopRunning();

        std::deque<std::function<void()>> notExecuted;
        m_safeQueue.copyQue(notExecuted);

        for (size_t i = 0; i < m_numOfThreads; i++) {
            m_safeQueue.pushFront(std::make_tuple(taskEnder, false));
        }

        RemoveAllThreads();
        return notExecuted;
    }
    return {};
}

size_t ThreadPoolExecutor::GetNumOfThreads() const
{
    // std::unique_lock<std::mutex> lock(m_mutex);
    return m_numOfThreads;
}

void ThreadPoolExecutor::CreateThreads(size_t a_numOfThreads) 
{
    std::unique_lock<std::mutex> lock(m_mutex);

    for (size_t i = 0; i < a_numOfThreads; i++) {
        m_workerThreads.emplace_back(std::thread(&ThreadPoolExecutor::Run, this));
    }
}

void ThreadPoolExecutor::Run()
{
    // std::unique_lock<std::mutex> lock(m_mutex);

    while (true)
    {
        try {
            std::tuple<std::function<void()>,bool> task;
            m_safeQueue.pop(task);
            (std::get<0>(task))();
        } catch(ExceptionClass const& ex) {
            m_threadId.pushBack(std::this_thread::get_id());
            break;
        } 
    }
}



void ThreadPoolExecutor::Clean(size_t a_numOfThreads)
{    
    for (size_t i = 0; i < a_numOfThreads; i++) {
        // std::unique_lock<std::mutex> lock(m_mutex);

        std::thread::id id;
        m_threadId.pop(id);

        auto it = std::find_if(m_workerThreads.begin(), m_workerThreads.end(), [&](const std::thread &t) -> bool
                               { return t.get_id() == id; });


        if (it->joinable()) {
            it->join();
            m_workerThreads.erase(it);
        }
    }
}


void ThreadPoolExecutor::RemoveAllThreads()
{
    for (auto &thread : m_workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    m_workerThreads.clear();
}

void ThreadPoolExecutor::StopRunning()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_running = false;
}
