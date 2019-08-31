#include "lib/http.hpp"

int main(int c , char** argv) {
    http::Server server(8080, "127.0.0.1");
    server.start();
    return 0;
}