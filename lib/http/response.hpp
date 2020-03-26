#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <string>

namespace http {
    namespace response {
        class Response {
            public:
            std::string status_code;
            std::string reason_phrase;
            std::string http_version;
        };


        enum status_codes {
            
        };
    }
}

#endif