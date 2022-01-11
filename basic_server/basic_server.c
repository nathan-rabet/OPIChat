#include "basic_server.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_CONNECTION_NB 5

int create_and_bind(struct addrinfo *addrinfo)
{
    for (; addrinfo != NULL; addrinfo = addrinfo->ai_next)
    {
        int s = socket(addrinfo->ai_family, addrinfo->ai_socktype,
                       addrinfo->ai_protocol);
        if (s == -1)
            continue;

        int ok = 1;
        if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int)) == -1
            || ok != 1)
            break;

        if (bind(s, addrinfo->ai_addr, addrinfo->ai_addrlen) != -1)
            return s;
    }

    exit(1);
}

int prepare_socket(const char *ip, const char *port)
{
    struct addrinfo hints = { 0 };
    struct addrinfo *addrinfo;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip, port, &hints, &addrinfo) != 0)
        exit(1);

    int server_socket = create_and_bind(addrinfo);

    freeaddrinfo(addrinfo);

    if (listen(server_socket, MAX_CONNECTION_NB) == -1)
        exit(1);

    return server_socket;
}

int accept_client(int socket)
{
    struct sockaddr client_addr;
    socklen_t addr_len;

    int client_socket = accept(socket, &client_addr, &addr_len);

    if (client_socket >= 0)
        printf("Client connected\n");

    return client_socket < 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        exit(1);
    }

    int socket = prepare_socket(argv[1], argv[2]);

    return accept_client(socket);
}
