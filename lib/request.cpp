#include "http.hpp"

void Request::add_header_from_string(std::string* raw_header) {
    std::string delimiter(": ");
    int split_pos = raw_header->find(delimiter);
    this->add_header(&(raw_header->substr(0, split_pos)), &(raw_header->substr(split_pos + 2, raw_header->size())));
}

void Request::add_header(std::string* key, std::string* value) {
    this->headers.insert(key, value);
}