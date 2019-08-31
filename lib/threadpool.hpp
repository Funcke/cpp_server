#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <vector>
#include <thread>
#include <queue>
#include <stdbool.h>

class Job {
public:
    virtual void run() = 0;
};

class Threadpool {
public:
    // properties
    std::mutex job_mutex;
    std::condition_variable condition;
    std::queue<Job*> job_queue;
    unsigned int size;
    std::vector<std::thread> pool;
    bool terminated;

    Threadpool(int size);
    ~Threadpool();
    // public methods
    void add_job(Job* j);

private:
    void worker_loop();
};
#endif