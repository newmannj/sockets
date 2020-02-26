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

int main(int argc, char** argv) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;

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

    while(1) {
        addr_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        if(new_fd < 0 ) {
            std::cout << "Accept failed, continuing.\n";
            continue;
        }
        char ip[128];
        inet_ntop(their_addr.ss_family, (struct sockaddr*)&their_addr, ip, sizeof(ip));
        std::cout << "Got connection from IP: " << ip << std::endl;
        memset(ip, 0, 128);
    }



}