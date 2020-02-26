#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>

#define PORT "8080"
#define BACKLOG 10
#define MAX_CLIENTS 5

int main(int argc, char** argv) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;
    int clients[MAX_CLIENTS] = {};
    char** ips = new char*[MAX_CLIENTS];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(argv[1], PORT, &hints, &res);
    struct sockaddr_in* this_addr = (struct sockaddr_in*)(res->ai_addr);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);
    std::cout << "Server listening on IP: " << inet_ntoa(this_addr->sin_addr) << " Port: " << ntohs(this_addr->sin_port) << std::endl;
    int client_count = 0;
    while(1) {
        addr_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);
        if(new_fd < 0 ) {
            std::cout << "Accept failed, continuing.\n";
            continue;
        }
        //Add new client to fd array.
        clients[client_count] = new_fd;
        char ip[128];
        inet_ntop(their_addr.ss_family, (struct sockaddr_in*)&their_addr, ip, sizeof(ip));
        std::cout << "Connection from client IP: " << ip << std::endl;
        //Store the ip in a char* so we can add to a list.
        char* next_client_ip = new char[128];
        for(size_t i = 0; i < 128; i++) {
            next_client_ip[i] = ip[i];
        }
        ips[client_count] = next_client_ip;
        client_count += 1;
        std::cout << "Informing other clients of new client." << std::endl;
        for(size_t i = 0; i < client_count - 1; i++) {
            int bytes_sent = send(clients[i], ip, 128, 0);
            if(bytes_sent < 0) {
                std::cout << "Message not sent." << std::endl;
            }
        }
        memset(ip, 0, 128);
        
        std::cout << "Informing newly connected client of other clients." << std::endl;
        for(size_t i = 0; i < client_count - 1; i++) {
            int bytes_sent = send(new_fd, ips[i], 128, 0);
        }
        
    }



}