#include "http.hpp"
#include <string>

void Request::add_header_from_string(std::string* raw_header) {
    std::string delimiter(": ");
    int split_pos = raw_header->find(delimiter);
    std::string key = raw_header->substr(0, split_pos);
    std::string value = raw_header->substr(split_pos + 2, raw_header->size());
    this->add_header(&key, &value);
}

void Request::add_header(std::string* key, std::string* value) {
    this->headers[*key] = *value;
}