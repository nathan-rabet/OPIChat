#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "init_socket.h"

static int create_and_connect(struct addrinfo *addrinfo)
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

        if (connect(s, addrinfo->ai_addr, addrinfo->ai_addrlen) != -1)
            return s;
    }

    return -1;
}

int setup_client_socket(const char *ip, const char *port)
{
    struct addrinfo hints;
    struct addrinfo *addrinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip, port, &hints, &addrinfo) != 0)
        return -1;

    int server_socket = create_and_connect(addrinfo);

    if (server_socket == -1)
        return -1;

    freeaddrinfo(addrinfo);
    return server_socket;
}
