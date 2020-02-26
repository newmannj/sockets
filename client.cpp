#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#define PORT "8080"
#define SERVER "127.0.0.1"

int main(int argc, char** argv) {
    struct addrinfo hints, *servinfo, *p;
    int sockfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(SERVER, PORT, &hints, &servinfo);

    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if(sockfd < 0) {
        std::cout << "Socket creation failed.\n";
        exit(1);
    }
    
    int connfd = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    if(connfd < 0) {
        std::cout << "Server connection failed.\n";
        exit(1);
    }
    std::cout << "Connected to server.\n";
    while(1) {
        char rbuff[128];
        int bytes_recieved = recv(sockfd, rbuff, 128, 0);
        if(bytes_recieved > 0) {
            std::cout << "Informed of client IP: " << rbuff << std::endl;
        }
    }



}