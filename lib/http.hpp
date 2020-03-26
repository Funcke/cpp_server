#ifndef __HTTP_H__
#define __HTTP_H__

#include <string>
#include <list>
#include <map>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <functional>


#include "threadpool.hpp"
namespace http {

    /**
     * Server represents a http server. It can be bound
     * to a TCP socket and will listen for incoming connections.
     */
    class Server {
    public:
        /**
         * Threadpool. Manages server threads for handling requests.
         */
        Threadpool* threadpool;
        /**
         * Socketdescriptor for HTTP socket
         */
        int network_socket;
        /**
         * network address configuration. Used to get the tcp socket from
         * operating system.
         */
        sockaddr_in network_access;

        /**
         * c'tor
         * @param port int - TCP port to be used
         * @param host std::string  - interface address to expose the server to
         */
        Server(int port, std::string host);

        /**
         * starts starts the server.
         * The start process consists of binding the process to the port
         * provided by the c'tor arguments and the host identifier.
         * After binding was successful, the server goes into main loop.
         * The main loop handles incoming connections and passes them into
         * the job queue.
         */
        void start();

        /**
         * stop stops the server.
         * When a stop signal is submitted, the server starts to free all
         * resources and closes all request connections.
         * If database connections are opened, end will also close them.
         */
        void stop();
    private:

        /**
         * connect_to_network binds network socket to the provided
         * network interface. This method is only used during
         * startup.
         * Checks if something goes wrong, the server is stopped
         * gracefully, providing error code and message.
         */
        void connect_to_network();

        /**
         * start_lifecycle starts listener loop.
         */
        void start_lifecycle();

        /**
         * wait_for_requests waits on the opened port for incoming requests.
         * If a client connected, the connection will be passed to
         * the job queue.
         */
        void wait_for_requests();

        /**
         * abort stops the running server.
         * @param code int - error code
         * @param message std::string - error message
         */
        void abort(int code, std::string message);
    };

    /**
     * Request represents a HTTP Request.
     * It is used to provide access to all data sent with
     * the incoming request. It will be initialized by the 
     * HttpRequestLifecycle and passed to the corresponding
     * event handler.
     */
    class Request {
    public:
        // properties
        /**
         * Request URI.
         * This is the URI string added after the hostname in the
         * request.
         */
        std::string path;
        /**
         * method contains the HTTP method used by the
         * the incoming request.
         */
        std::string method;
        /**
         * Contains version string of HTTP
         */
        std::string protocol;
        /**
         * map with HTTP headers passed to the request.
         * If the desired header contains data in a special
         * datatype, it has to be converted.
         */
        std::map<std::string, std::string> headers;
        /**
         * map with cookies stored in the client.
         * The cookies will be delivered with the HTTP request
         * and are stored in string format.
         * If the stored value has a different data type,
         * it has to be casted.
         */
        std::map<std::string, std::string> cookies;

        // Methods
        /**
         * add_header_from_string takes header as single string
         * in format: "name: value\n". It will be split and added
         * to the headers map.
         * @param raw_header std:.string* - the raw_header
         */
        void add_header_from_string(std::string* raw_header);
        
        /**
         * first_line is used to intialize HTTP request method, HTTP URI
         * and HTTP protocol version.
         * @param line std::string* - the raw first line of a HTTP request
         */
        void first_line(std::string* line);
    private:
        /**
         * add_header adds header to headers map.
         * @param key std::string* - the identifier for the header
         * @param value std::string* - the value of the header
         */
        void add_header(std::string* key, std::string* value);

        /**
         * add_cookie adds cookie to the cookie map.
         * As the cookie will be passed as single string in
         * format "name=value" the method splits it and retrieves
         * name and value as saparate strings. The name will be used
         * to access the value from the map.
         * @param cookie std::string* - the raw cookie value from Cookie-header
         */
        void add_cookie(std::string* cookie);
    };
    
    /**
     * HttpRequestLifecycle represent the whole lifecycle,
     * a request goes through before it is sent back to the
     * client. 
     */
    class HttpRequestLifecycle: public Job {
    public:
        HttpRequestLifecycle(int socket);
        void run();
    private:
        Request request;
        int socket;
        std::string* read_line();
        //Response response;
    };

    class RequestHandler {
    public:
        RequestHandler(Request* request);
        void run();

    private:
        Request* request;
        std::map<std::string, std::function<void(Request*)>> handler_map;
    };
}
#endif