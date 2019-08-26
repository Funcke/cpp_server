#ifndef __HTTP_H__
#define __HTTP_H__

#include <string>
#include <list>
#include <map>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "threadpool.hpp"

class Server {
public: 
    int port;
    std::string host;
    Threadpool threadpool{120};
    int network_socket;
    sockaddr_in network_access;
    std::thread* next_socket;

    Server(int port, std::string host);
    void start();
    void stop();
private:
    void connect_to_network();
    void abort(int code, std::string* message);
    void start_lifecycle();
};


class RequestHandler: public Job {
    RequestHandler(Request* request);
    void run();

private:
    Request* request;
    std::map<std::string, std::function<void(Request* req)>> handler_map;
};
#endif