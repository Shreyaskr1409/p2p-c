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

    struct LinkedList *known_hosts = arg;
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 5858, 20);
    struct sockaddr *addr = (struct sockaddr *)&server.address;
    socklen_t addr_len = (socklen_t)sizeof(server.address);

    while (1) {
        int fd = accept(server.socket, addr, &addr_len);
        char request[255];
        memset(request, 0, 255);
        read(fd, request, 255);
        char *client_address = inet_ntoa(server.address.sin_addr);
        printf("\t\t\t%s says: %s\n", client_address, request);
        close(fd);

        short found = 0;
        for (int i = 0; i < known_hosts->length && !found; i++) {
            if (strcmp(client_address, known_hosts->retrieve(known_hosts, i)) == 0) {
                found = 1;
            }
        }
        if (!found) {
            known_hosts->insert(known_hosts, known_hosts->length, client_address, sizeof(client_address));
        }
    }
    return NULL;
}

void client_function(char *request, struct LinkedList *known_hosts) {
    struct Client client = client_constructor(AF_INET, SOCK_STREAM, 0, 5858, INADDR_ANY);
    for (int i = 0; i < known_hosts->length; i++) {
        client.request(&client, known_hosts->retrieve(known_hosts, i), request, 255);
    }
}

int main() {
    struct LinkedList known_hosts = linked_list_constructor();
    known_hosts.insert(&known_hosts, 0, "127.0.0.1", 10);

    printf("Starting server...\n");
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, server_function, &known_hosts);

    while (1) {
        char request[255];
        memset(request, 0, 255);
        fgets(request, 255, stdin);
        client_function(request, &known_hosts);
    }
}
