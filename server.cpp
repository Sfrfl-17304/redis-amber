#include <unistd.h>
#include <cstdlib>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


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



}
