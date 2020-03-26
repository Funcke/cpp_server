#include "http.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>

namespace http {
    HttpRequestLifecycle::HttpRequestLifecycle(int socket) {
        this->socket = socket;
        this->request.first_line(this->read_line());
        printf("%s\n", this->request.protocol.c_str());
        while(true) {
            std::string* raw_header = this->read_line();
            if(raw_header->length() == 1)
                break;
            this->request.add_header_from_string(raw_header);
        }
    }

    void HttpRequestLifecycle::run() {
        printf("%d\n", this->request.headers.size());
        close(this->socket);
    }

    std::string* HttpRequestLifecycle::read_line() {
        std::string* line = new std::string();
        char next;
        do{
        read(this->socket, &next, 1); 
        if(next != '\n')
            *line += next;
        }while(next != '\n');
        return line;
    }
}