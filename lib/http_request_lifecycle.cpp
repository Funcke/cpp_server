#include "http.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <string>

HttpRequestLifecycle::HttpRequestLifecycle(int socket) {
    char buffer[getpagesize()];
    while(read(socket, &buffer, getpagesize())) {
        printf(buffer);
        this->request.add_header_from_string(&std::string(buffer));
    }
    this->socket = socket;
}

void HttpRequestLifecycle::run() {
    printf(this->request.headers[0].c_str());
    close(this->socket);
}