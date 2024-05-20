#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>
#include <atomic>
#include <functional>
#include <unistd.h>
#include <vector>
#include <queue>
#include <sys/syscall.h>

/**
 * @class:ThreadPool
 * @brief:线程池类
*/

class ThreadPool
{
private:
    size_t m_threadnums = 2;                            //线程池线程数量
    std::vector<std::thread> m_threads;                 //线程池中的线程
    std::queue<std::function<void()>> m_taskqueue;      //任务队列
    std::mutex m_mutex;                                 //任务队列同步互斥锁
    std::condition_variable m_cv;                       //任务队列同步的条件变量
    std::atomic_bool m_stop;                            //退出线程的标志
    std::string m_type;                                 //线程种类“IO” “Work”
public:
    ThreadPool();
    ThreadPool(size_t threadsnum, const std::string& type = "IO");
    ~ThreadPool();

    void addTask(std::function<void()> task);
};


#endif