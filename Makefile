server: main.cpp lib/http_request_lifecycle.cpp lib/request_handler.cpp lib/request.cpp lib/server.cpp lib/threadpool.cpp
	g++  main.cpp lib/http_request_lifecycle.cpp lib/request_handler.cpp lib/request.cpp lib/server.cpp lib/threadpool.cpp -std=c++17 -lpthread -o main
