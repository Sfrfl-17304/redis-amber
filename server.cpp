#include <string>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>


int main() {
    // Initialize socket & Port
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int port = 6379;
    // Socket creation check
    if (sock == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Socket address initialization
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    // Address setup
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_result = bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    // Bind check
    if (bind_result == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    int listen_result = listen(sock, 1);

    // Listen check
    if (listen_result == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //client address init
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    //client address length
    socklen_t client_addr_len = sizeof(client_addr);
    //Accept connection
    int accept_result = accept(sock, (struct sockaddr*)&client_addr, &client_addr_len);

    if (accept_result == -1){
        perror("accept");
        exit(EXIT_FAILURE);
    }

char buffer[1024];
size_t total_read = 0;
while(1){
    ssize_t n = read(accept_result, buffer + total_read, sizeof(buffer)-total_read-1);
    if (n == -1){
        perror("read");
        exit(EXIT_FAILURE);
    }


    if (n > 0){
        total_read += n;
        buffer[total_read] = '\0';
        printf("got something %s\n", buffer + total_read - n);
        std::string response = "";
        ssize_t write_result = 0;
        if (strncmp(buffer + total_read - n, "PING", 4) == 0){
            printf("returning PONG...\n");
            response = "PONG\r\n";
            write_result = write(accept_result, response.c_str(), response.length()+1);

        } else if (strncmp(buffer + total_read - n, "ECHO", 4) == 0){
            response = (buffer + total_read - n) + 5;
            write_result = write(accept_result, response.c_str(), response.length()+1);
        }

        if (write_result == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }


    }
    if (n == 0) {
        printf("no data");
    }
}

}
