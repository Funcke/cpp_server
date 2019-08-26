#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <vector>
#include <thread>
#include <stdbool.h>

class Threadpool {
public:
    // properties
    std::mutex job_mutex;
    std::condition_variable condition;
    std::vector<Job*> job_queue;
    unsigned int size;
    std::vector<std::thread> pool;
    int terminated = false;

    Threadpool(int size);
    // public methods
    void add_job(Job* j);

private:
    void worker_loop();
};

class Job {
public:
    virtual void run();
};
#endif