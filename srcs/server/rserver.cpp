/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rserver.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rimney < rimney@student.1337.ma>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 04:16:59 by rimney            #+#    #+#             */
/*   Updated: 2023/03/09 05:07:07 by rimney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/parsing.hpp"
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

int main()
{
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening == -1)
    {
        std::cerr << "Can't Create A Socket\n";
        exit(1);
    }
    sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if(bind(listening, (sockaddr *)&addr, sizeof(addr)) == -1)
    {
        std::cerr << "Can't bind to Ip/Port\n";
        exit(1);
    }
    if(listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can't Listent to Socket\n";
        exit(1);
    }

    sockaddr_in client;
    socklen_t  ClientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket = accept(listening, (sockaddr *)&clientSocket, &ClientSize);
    if(clientSocket == -1)
    {
        std::cerr << "there was a problem when client is connecting\n";
        exit(1);
    }
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr *)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result)
    {
        std::cout << host << "connected on " << svc  << '\n'; 
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << htons(client.sin_port) << '\n';
    }
    char buf[4096];
    while (true) {
    memset(buf, 0, 4096);
    int received = recv(clientSocket, buf, 4096, 0);
    if (received == -1) {
        std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
        exit(1);
    } else if (received == 0) {
        std::cout << "The client disconnected." << std::endl;
        break;
    } else {
        std::string request = buf;
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello world!";
        send(clientSocket, response.c_str(), response.size(), 0);
    }
}
    close(clientSocket);
    
}