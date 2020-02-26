all: server client

server: server.cpp
	g++ -std=c++11 -Wall server.cpp -o server

client: client.cpp
	g++ -std=c++11 -Wall client.cpp -o client