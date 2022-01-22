#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "epoll_handler.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_socket = prepare_socket(argv[1], argv[2]);

    int epoll_instance = epoll_create1(0);
    if (epoll_instance == -1)
        raise_panic(EXIT_FAILURE, "Impossible to create epoll instance");

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_socket;

    if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, server_socket, &event) == -1)
        raise_panic(EXIT_FAILURE,
                    "Impossible to add server socket to epoll instance : %s",
                    strerror(errno));

    struct connection_t *connections = NULL;
    struct epoll_event events[MAX_EVENTS] = { 0 };
    while (true)
    {
        int nb_events = epoll_wait(epoll_instance, events, 1, -1);
        if (nb_events == -1)
            write_error("Impossible to wait for events : %s", strerror(errno));

        for (int i = 0; i < nb_events; i++)
        {
            if (events[i].data.fd == server_socket)
                connections =
                    accept_client(epoll_instance, server_socket, connections);
            else
                connections =
                    communicate(epoll_instance, events[i].data.fd, connections);
        }
    }
    return 0;
}
