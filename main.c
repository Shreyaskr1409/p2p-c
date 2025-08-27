#include "libeom/Networking/Nodes/Client.h"
#include "libeom/Networking/Nodes/Server.h"
#include <libeom/libeom.h>
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

void * server_function(void *arg) {
    printf("Server running.\n");
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 5858, 20);
    struct sockaddr *addr = (struct sockaddr *)&server.address;
    socklen_t addr_len = (socklen_t)sizeof(server.address);
    while (1) {
        int fd = accept(server.socket, addr, &addr_len);
        char request[255];
        memset(request, 0, 255);
        read(fd, request, 255);
        printf("\t\t\t%s says: %s\n", inet_ntoa(server.address.sin_addr), request);
        close(fd);
    }
    return NULL;
}

void client_function(char *request) {
    struct Client client = client_constructor(AF_INET, SOCK_STREAM, 0, 5858, INADDR_ANY);
    client.request(&client, "127.0.0.1", request, 255);
}

int main() {
    printf("Starting server...\n");
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, server_function, NULL);

    while (1) {
        char request[255];
        memset(request, 0, 255);
        fgets(request, 255, stdin);
        client_function(request);
    }
}
