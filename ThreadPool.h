#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);

    ~ThreadPool();

    void addTask(const function<void()>& task);

private:
    void workerThread();

    vector<thread> workers;
    queue<function<void()>> tasks;

    mutex queueMutex;
    condition_variable condition;
    bool stop;
};
