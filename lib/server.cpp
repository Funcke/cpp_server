#include "http.hpp"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <string>

namespace http {
    /**
     * c'tor
     * Initializes internet address struct and server threadpool
     * @param port int - network port that should be exposed
     * @param host std::string - network address(resembles network interface)
     *                           to expose the selected port to.
     */
    Server::Server(int port, std::string host) {
        this->network_access.sin_port = htons(port);
        this->network_access.sin_addr.s_addr = inet_addr(host.c_str());
        this->network_access.sin_family = AF_INET;
        this->threadpool = new Threadpool(120);
    }

    /**
     * start exposes port and starts the server lifecycle.
     */
    void Server::start() {
        this->connect_to_network();
        this->start_lifecycle();
    }

    /**
     * connect_to_network connects application server to network.
     */
    void Server::connect_to_network() {
        //memset(&this->network_access, 0, sizeof(this->network_access));
        this->network_socket = socket( AF_INET, SOCK_STREAM, 0 );
        
        if(bind(this->network_socket, (struct sockaddr*)&this->network_access, sizeof(this->network_access)) > 0) {
            this->abort(-1, "Could not bind server to socket!\n");
        }

        if(listen(this->network_socket, 100) < 0) {
            this->abort(-2, "Connection queue overrun!\n");
        }
    }

    /**
     * start_lifecycle creates a main application loop.
     * This loop waits for all incoming requetsts and passes them
     * as jobs to the job queue.
     */
    void Server::start_lifecycle() {
        for(;;) {
            this->wait_for_requests();
        }
    }

    /**
     * wait_for_requests literally waits for requests.
     * I know. I am the king of naming.
     * It uses the socket opened in connect_to_network to wait for incoming
     * requests. If a client fails to connect, the method will kill the server.
     * If everything goes well, the new connection is passed on to the job queue.
     */
    void Server::wait_for_requests() {
        socklen_t len;
        len = sizeof(this->network_access);
        int connection = accept(this->network_socket, (struct sockaddr*)&this->network_access, &len);
        if(connection < 0) {
            this->abort(-3, "Client could not connect\n");
        }
        HttpRequestLifecycle* lc = new HttpRequestLifecycle(connection);
        this->threadpool->add_job(lc);
    }

    /**
     * Should someday gracefully kill the server without blocking
     * system resources.
     * @param status int - error code to print
     * @param std::string message - error message to print out
     */
    void Server::abort(int status, std::string message) {
        fprintf(stderr, "%s", message.c_str());
    }
}