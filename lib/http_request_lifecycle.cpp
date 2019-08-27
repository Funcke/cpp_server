#include "http.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>

HttpRequestLifecycle::HttpRequestLifecycle(int socket) {
    this->socket = socket;
    while(true) {
        std::string* raw_header = this->read_line();
        std::cout << raw_header->length() << std::endl;
        if(raw_header->length() == 1)
            break;
        this->request.add_header_from_string(raw_header);
        printf("Added header\n");
    }
    printf("Last read from header\n");
}

void HttpRequestLifecycle::run() {
    printf("%d", this->request.headers.size());
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