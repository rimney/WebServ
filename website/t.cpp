#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 4096 // size of each chunk

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if (sockfd == -1) {
    //     std::cerr << "Failed to create socket" << std::endl;
    //     return 1;
    // }

    // set up server address and port
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(8085);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        close(sockfd);
        return 1;
    }

    std::string filename = "me.jpg";
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file" << std::endl;
        close(sockfd);
        return 1;
    }

    // calculate file size
    file.seekg(0, std::ios::end);
    uint64_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // send file size
    if (send(sockfd, &file_size, sizeof(file_size), 0) == -1) {
        std::cerr << "Failed to send file size" << std::endl;
        close(sockfd);
        return 1;
    }

    // send file data in chunks
    std::vector<char> buffer(BUFFER_SIZE);
    while (file) {
        file.read(buffer.data(), buffer.size());
        int bytes_sent = send(sockfd, buffer.data(), file.gcount(), 0);
        if (bytes_sent == -1) {
            std::cerr << "Failed to send data" << std::endl;
            close(sockfd);
            return 1;
        }
    }

    file.close();
    close(sockfd);
    return 0;
}