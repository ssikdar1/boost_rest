#g++ -I/home/shan/asio/include/boost/asio  main.cpp connection.cpp connection_manager.cpp mime_types.cpp reply.cpp request_handler.cpp request_parser.cpp server.cpp -std=c++11 -pthread -lboost_system

src = $(wildcard *.cpp)
obj = $(src:.c=.o)
#CC = g++ -I/home/shan/asio/include/boost/asio  -std=c++11 -g -Wall
CC = g++ -std=c++11 -g -Wall
LDFLAGS = -pthread -lboost_system 

shan_server : $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f shan_server 
