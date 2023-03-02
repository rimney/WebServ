#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <string>
#include "/Users/brmohamm/Desktop/WebServ/includes/request.hpp"

#define PORT 8080

//http://localhost:8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char* ip = "127.0.0.1";
    
   char * hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nVON";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons( PORT );
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }        
    Request request;
    while(1)
    {
        std::string tmp;
        std::cout << "\nWaiting for new connection ...\n\n" << std::endl;
        std::string st = "GET /favicon.ico HTTP/1.1\nHost: localhost:8080\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:109.0) Gecko/20100101 Firefox/110.0\nAccept: image/avif,image/webp,*/*\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate, br\nConnection: keep-alive\nReferer: http://localhost:8080/\nSec-Fetch-Dest: image\nSec-Fetch-Mode: no-cors\nSec-Fetch-Site: same-origin";
        request = st;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        char buffer[30000] = {0};
        recv(new_socket,buffer,30000,0);
        tmp = buffer;
        send(new_socket,hello  ,strlen(hello) ,0);
        std::cout << "* Hello message sent *"<< std::endl;
        close(new_socket);
    }
    return 0;
}