#include "http.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>

namespace http {
    void Request::first_line(std::string* first) {
        std::istringstream iss(*first);
        std::vector<std::string> results(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>());
        this->method = results[0];
        this->path = results[1];
        this->protocol = results[2];                   
    }
    void Request::add_header_from_string(std::string* raw_header) {
        const std::string delimiter(": ");
        int split_pos = raw_header->find(delimiter);
        std::string key = raw_header->substr(0, split_pos);
        std::string value = raw_header->substr(split_pos + 2, raw_header->size());
        if(key == "Cookie")
            this->add_cookie(&value);
        else
            this->add_header(&key, &value);
        std::cout << key << std::endl;
    }

    void Request::add_header(std::string* key, std::string* value) {
        this->headers[*key] = *value;
    }

    void Request::add_cookie(std::string* cookie) {
        const std::string delimiter("=");
        int split_pos = cookie->find(delimiter);
        const std::string key = cookie->substr(0, split_pos);
        const std::string value = cookie->substr(split_pos + 1, cookie->size());
        this->cookies[key] = value;
    }
}