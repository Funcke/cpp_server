#ifndef __HTTP_H__
#define __HTTP_H__

#include <string>
#include <list>
#include <map>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <functional>

#include "threadpool.hpp"

class Server {
public: 
    int port;
    std::string host;
    Threadpool* threadpool;
    int network_socket;
    sockaddr_in network_access;
    std::thread* next_socket;

    Server(int port, std::string host);
    void start();
    void stop();
private:
    void connect_to_network();
    void abort(int code, std::string message);
    void start_lifecycle();
};

class Request {
public:
    std::map<std::string, std::string> headers;
    void add_header_from_string(std::string* raw_header);
    void add_header(std::string* key, std::string* value);
};

class HttpRequestLifecycle: public Job {
public:
    HttpRequestLifecycle(int socket);
    void run();
private:
    Request request;
    int socket;
    std::string* read_line();
    //Response response;
};

class RequestHandler {
public:
    RequestHandler(Request* request);
    void run();

private:
    Request* request;
    std::map<std::string, std::function<void(Request*)>> handler_map;
};


#endif