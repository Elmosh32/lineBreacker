#include "periodicExecutor.hpp"

// implement phase 1
// - add task
// - run
// - shutdown
// implement phase 2
// - pause
// - resume

PeriodicExecutor::PeriodicExecutor()
: m_thread(std::move(std::thread(&PeriodicExecutor::run, this)))
{}

void PeriodicExecutor::addTask(Task &a_task)
{
    m_queue.push(a_task);
}

void PeriodicExecutor::addTask(Task &&a_task)
{
    m_queue.push(std::move(a_task));
}

void PeriodicExecutor::shutdown()
{
    m_stop = true;
    m_thread.join();
}

void PeriodicExecutor::pause(std::chrono::milliseconds a_milTime)
{
    pause();
    std::this_thread::sleep_for(a_milTime);
    resume();
}

void PeriodicExecutor::pause()
{
    std::cout << "pause()\n";
    m_pause.store(true);
    m_cv.notify_one();
}

void PeriodicExecutor::resume()
{
    std::cout << "resume\n";
    m_pause.store(false);
    m_cv.notify_one();
}

void PeriodicExecutor::waitForSignal()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this]{
        return !m_pause;
    });
}

void PeriodicExecutor::run()
{
    while (!m_stop) {
        waitForSignal();

        Task task;
        m_queue.pop(task);

        for (int counter = 0; ; counter++) {
            try {
                if (task.runTask()) {
                    m_queue.push(task);
                }
                break;
            } catch (...) {
                if (counter == 3) {
                    break;
                }
            }
        }
    }
}