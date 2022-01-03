#include "basic_client.h"

#include <stdlib.h>
#include <unistd.h>

int create_and_connect(struct addrinfo *addrinfo)
{
    for (; addrinfo != NULL; addrinfo = addrinfo->ai_next)
    {
        int s = socket(addrinfo->ai_family, addrinfo->ai_socktype,
                       addrinfo->ai_protocol);
        if (s == -1)
            continue;

        if (connect(s, addrinfo->ai_addr, addrinfo->ai_addrlen) != -1)
            return s;
    }

    exit(1);
}

int prepare_socket(const char *ip, const char *port)
{
    struct addrinfo hints;
    struct addrinfo *addrinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip, port, &hints, &addrinfo) != 0)
        exit(1);

    int server_socket = create_and_connect(addrinfo);

    freeaddrinfo(addrinfo);
    return server_socket;
}

void communicate(int server_socket)
{
    char buf[DEFAULT_BUFFER_SIZE] = { 0 };

    ssize_t read_len = 0;
    while ((read_len = read(STDIN_FILENO, &buf, DEFAULT_BUFFER_SIZE)) != 0)
    {
        if (read_len == -1)
            exit(1);

        ssize_t i = 0;
        char hasLF = 0;
        for (; i < read_len && !hasLF; i++)
            hasLF = (buf[i] == '\n');

        ssize_t send_len = 0;
        ssize_t error = 0;
        while ((error = send(server_socket, buf + send_len,
                             read_len - send_len - hasLF, MSG_MORE))
               != 0)
        {
            if (error == -1)
                exit(1);
            send_len += error;
        }

        if (hasLF && send(server_socket, "\n", 1, MSG_EOR) < 1)
            exit(1);
    }
}
