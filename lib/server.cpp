#include "http.hpp"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <string>

Server::Server(int port, std::string host) {
    this->port = port;
    this->host = host;
}

void Server::start() {
    this->connect_to_network();
    this->start_lifecycle();
}

void Server::connect_to_network() {
    memset(&this->network_access, 0, sizeof(&this->network_access));
    this->network_access.sin_family = AF_INET;
    this->network_access.sin_addr.s_addr = inet_addr(this->host.c_str());
    this->network_access.sin_port = htons(this->port);
    this->network_socket = socket( AF_INET, SOCK_STREAM, 0 );
    
    if(bind(this->network_socket, (struct sockaddr*)&this->network_access, sizeof(this->network_access)) > 0) {
        this->abort(-1, &std::string("Could not bind server to socket!\n"));
    }

    if(listen(this->network_socket, 100) < 0) {
        this->abort(-2, &std::string("Connection queue overrun!\n"));
    }
}

void Server::start_lifecycle() {
    for(;;) {
        socklen_t len;
        len = sizeof(this->network_access);
        int connection = accept(this->network_socket, (struct sockaddr*)&this->network_access, &len);
        if(connection < 0) {
            this->abort(-3, &std::string("Client could not connect\n"));
        }
        
        //this->threadpool
        char buffer[10000];
        int read_data_size;

        read_data_size = read (connection, buffer, 10000);
        if (read_data_size < 0)
        {
            this->abort(-4, &std::string("Error getting data from client\n"));
        }
        else if (read_data_size == 0)
            close(connection);
        else
        {
            /* Data read. */
            fprintf (stderr, "Server: got message: `%s'\n", buffer);
        }
    }
}