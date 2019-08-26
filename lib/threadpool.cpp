#include "threadpool.hpp"

Threadpool::Threadpool(int size) {
    this->size = size;
    this->terminated = false;
    for(int i = 0; i < size; i++) {
        this->pool.push_back(std::thread(&(this->worker_loop)));
    }
}

void Threadpool::add_job(Job* j) {
    {
        std::unique_lock<std::mutex> lock(this->job_mutex);
        this->job_queue.push_back(j);
    }
    this->condition.notify_one();
}

void Threadpool::worker_loop() {
    while(1) {
        Job j;
        {
            std::unique_lock<std::mutex> lock(this->job_mutex);
            std::condition_variable().wait(lock, [&]{return !this->job_queue.empty() || terminated;});
            j = *this->job_queue.front();
        }
        j.run();
    }
}