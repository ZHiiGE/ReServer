#include "ThreadPool.h"
#include <iostream>
/**
 * @class:ThreadPool
 * @brief:线程池类
*/

ThreadPool::ThreadPool():ThreadPool(2){

}

ThreadPool::ThreadPool(size_t threadsnum):m_threadnums(threadsnum), m_stop(false){
    //启动m_threadsnum数量的线程,并阻塞在等待条件变量处
    for(int ii=0;ii<m_threadnums;++ii){

        
        auto fn = [this](){
            printf("create thread(%ld)\n", syscall(SYS_gettid));
            // std::cout << "子线程：" << std::this_thread::get_id() << std::endl;

            while (m_stop == false){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->m_mutex);
                    //等待任务队列同步
                    this->m_cv.wait(lock, [this]{return this->m_stop || !this->m_taskqueue.empty();});
                    //队列中没有任务才能退出线程
                    if(this->m_stop && this->m_taskqueue.empty())
                        return;
                    task = std::move(this->m_taskqueue.front()); 
                    this->m_taskqueue.pop();
                }
                printf("thread(%ld) runing\n", syscall(SYS_gettid));
                task();
            }
            
        };

        m_threads.emplace_back(fn);
    }
}

ThreadPool::~ThreadPool()
{
    m_stop = true;
    m_cv.notify_all();
    for(auto &th:m_threads){
        th.join();
    }
}

void ThreadPool::addTask(std::function<void()> task){
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_taskqueue.push(task);
    }
    m_cv.notify_one();
}


// void show(int no, std::string &name){
//     std::cout<<"hello my name is"<<name<<". no is"<<no<<std::endl;
// }

// void test(){
//     std::cout<<"hello my name is test"<<std::endl;
// }

// int main(){
//     ThreadPool threadPool(3);
//     sleep(1);
//     std::string name("xishi");
//     threadPool.addTask(std::bind(test));
//     sleep(1);
//     threadPool.addTask(std::bind(show, 2, name));
//     sleep(1);
//     threadPool.addTask(std::bind(show, 4, name));
// }

