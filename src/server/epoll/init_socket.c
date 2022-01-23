#include "init_socket.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"

/**
 * @brief Init the socket system and bind it
 *
 * @param addrinfo The address info to use
 * @return The created socket, exit on error
 */
static int _setup_socket_basis(struct addrinfo *addrinfo)
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

    raise_panic(EXIT_FAILURE, "Could not bind socket to %s",
                addrinfo->ai_canonname);

    return -1;
}

int setup_server_socket(const char *ip, const char *port)
{
    struct addrinfo hints = { 0 };
    struct addrinfo *addrinfo;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip, port, &hints, &addrinfo) != 0)
        raise_panic(EXIT_FAILURE, "Could not get address info for %s:%s", ip,
                    port);

    int server_socket = _setup_socket_basis(addrinfo);

    freeaddrinfo(addrinfo);

    if (listen(server_socket, BACKLOG) == -1)
        raise_panic(EXIT_FAILURE, "Could not listen on socket %d",
                    server_socket);

    return server_socket;
}
