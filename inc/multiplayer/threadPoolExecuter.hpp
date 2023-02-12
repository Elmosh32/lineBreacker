#ifndef THREAD_POOL_EXECUTER
#define THREAD_POOL_EXECUTER

#include <vector>
#include <thread>
#include <functional>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <tuple>

#include "./inl/safeQueue.hxx"

class ThreadPoolExecutor
{
public:
    explicit ThreadPoolExecutor(size_t a_numOfThreads);
    ~ThreadPoolExecutor();
    ThreadPoolExecutor(ThreadPoolExecutor const &) = delete;

    void Submit(std::function<void()> &a_task);
    void Submit(std::function<void()> &&a_task);

    void Add(size_t a_numOfThreads);
    void Remove(size_t a_numOfThreads);

    void Shutdown();
    std::deque<std::function<void()>> ShutdownNow();

    size_t GetNumOfThreads() const;

private:
    void CreateThreads(size_t a_numOfThreads);
    void Run();
    void StopRunning();
    void RemoveAllThreads();
    void RemoveThreads(size_t a_numOfThreads);
    void Clean(size_t a_numOfThreads);
    void removeNo(size_t a_numOfThreads);

    class ExceptionClass : public std::logic_error
    {
    public:
        ExceptionClass() : std::logic_error("ExceptionClass occurred!") {}
    };

    static void taskEnder()
    {
        throw ExceptionClass();
    }

private:
    std::vector<std::thread> m_workerThreads;
    SafeQueue<std::tuple<std::function<void()>, bool>> m_safeQueue;
    SafeQueue<std::thread::id> m_threadId;
    std::mutex m_mutex;
    size_t m_numOfThreads;
    bool m_running = true;
};

#endif /* THREAD_POOL_EXECUTER */
