#ifndef THREAD_DEAL_H
#define THREAD_DEAL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <vector>
#include <atomic>

/**
 * 线程池类
 * 提供多线程任务处理能力
 */
class ThreadPool {
public:
    /**
     * 构造函数
     * @param num_threads 线程数量
     */
    explicit ThreadPool(size_t num_threads);

    /**
     * 析构函数
     */
    ~ThreadPool();

    /**
     * 添加任务到线程池
     * @param task 要执行的任务
     */
    void enqueue(std::function<void()> task);

    /**
     * 等待所有任务完成
     */
    void waitForCompletion();

private:
    // 工作线程函数
    void workerThread();

    std::vector<std::thread> workers_; // 工作线程
    std::queue<std::function<void()>> tasks_; // 任务队列
    std::mutex queue_mutex_; // 队列互斥锁
    std::condition_variable condition_; // 条件变量
    std::atomic<bool> stop_; // 是否停止
    std::atomic<size_t> active_tasks_; // 活跃任务数
    std::mutex active_tasks_mutex_; // 活跃任务数互斥锁
};

#endif // THREAD_DEAL_H