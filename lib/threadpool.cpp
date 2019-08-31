#include "threadpool.hpp"

Threadpool::Threadpool(int size) {
    this->size = size;
    this->terminated = false;
    for(int i = 0; i < size; i++) {
        this->pool.push_back(std::thread(&Threadpool::worker_loop, this));
    }
}

Threadpool::~Threadpool() {
    delete(&(this->job_mutex));
    delete(&(this->condition));
    delete(&(this->job_queue));
    delete(&(this->size));
    delete(&(this->pool));
    delete(&(this->terminated));
}

void Threadpool::add_job(Job* j) {
    std::unique_lock<std::mutex> lock(this->job_mutex);
    this->job_queue.push(j);
    printf("Job added\n");
    lock.unlock();
    this->condition.notify_all();
}

void Threadpool::worker_loop() {
    while(1) {
        {
            Job* j;
            std::unique_lock<std::mutex> lock(this->job_mutex);
            this->condition.wait(lock, [this]{ return (this->job_queue.size() != 0 || this->terminated);});
            j = std::move(this->job_queue.front());
            this->job_queue.pop();
            printf("Job read\n");
            lock.unlock();
            j->run();
        }
    }
}