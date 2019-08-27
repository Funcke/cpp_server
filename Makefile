server: main.cpp lib/http_request_lifecycle.cpp lib/request_handler.cpp lib/request.cpp lib/server.cpp lib/threadpool.cpp
	g++ -std=c++14 -o main main.cpp lib/http_request_lifecycle.cpp lib/request_handler.cpp lib/request.cpp lib/server.cpp lib/threadpool.cpp -I
