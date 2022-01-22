#include "epoll-server.h"

#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "utils/xalloc.h"

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

struct connection_t *accept_client(int epoll_instance, int server_socket,
                                   struct connection_t *connection)
{
    struct sockaddr client_addr;
    socklen_t addr_len;

    int client_socket = accept(server_socket, &client_addr, &addr_len);

    if (client_socket == -1)
        return connection;

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = client_socket;
    if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, client_socket, &event) == -1)
        exit(1);

    return add_client(connection, client_socket);
}

struct connection_t *delete_epoll_client(struct connection_t *connection,
                                         int epoll_instance, int client_socket)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = client_socket;
    if (epoll_ctl(epoll_instance, EPOLL_CTL_DEL, client_socket, &event) == -1)
        exit(1);

    return remove_client(connection, client_socket);
}

struct connection_t *communicate(int epoll_instance, int client_socket,

                                 struct connection_t *connections)
{
    struct connection_t *client = find_client(connections, client_socket);
    client->buffer = xmalloc(DEFAULT_BUFFER_SIZE);

    bool hasLF = 0; // Has any '\n' ?
    ssize_t buf_mult_factor = 1; // buf[DEFAULT_BUFFER_SIZE * buf_mult_factor]

    ssize_t read_len; // Number returned by read
    while ((read_len = recv(client_socket, client->buffer + client->nb_read,
                            DEFAULT_BUFFER_SIZE, 0))
           != 0)
    {
        // If any client reading error
        if (read_len == -1)
        {
            fprintf(stderr, "Error while reading client data\n");
            return delete_epoll_client(connections, epoll_instance,
                                       client_socket);
        }

        // Determines real message length (must end with a '\n)
        for (ssize_t i = 0; !hasLF && i < read_len; i++)
            hasLF = (client->buffer[client->nb_read++] == '\n');

        // If '\n' detected
        if (hasLF)
            break;

        // The next loop can overflow the buffer
        else
        {
            client->buffer = realloc(client->buffer,
                                     (++buf_mult_factor) * DEFAULT_BUFFER_SIZE);
            if (!client->buffer)
            {
                fprintf(stderr, "Error while reallocating memory\n");
                return delete_epoll_client(connections, epoll_instance,
                                           client_socket);
            }
        }
    }

    if (!hasLF)
        return delete_epoll_client(connections, epoll_instance, client_socket);
    // Broadcast to all clients
    struct connection_t *parser = connections;
    while (parser != NULL)
    {
        int send_len = 0;

        while (send_len != client->nb_read)
        {
            int error = 0;
            error = send(parser->client_socket, client->buffer + send_len,
                         client->nb_read - send_len, MSG_NOSIGNAL & 0);

            // If any client sending error (e.g. client disconnected)
            if (error == -1)
                break;
            send_len += error;
        }
        parser = parser->next;
    }

    return connections;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    int server_socket = prepare_socket(argv[1], argv[2]);

    int epoll_instance = epoll_create1(0);
    if (epoll_instance == -1)
        exit(1);

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_socket;

    if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, server_socket, &event) == -1)
        exit(1);

    struct connection_t *connections = NULL;
    struct epoll_event events[MAX_EVENTS] = { 0 };
    while (true)
    {
        int nb_events = epoll_wait(epoll_instance, events, 1, -1);
        if (nb_events == -1)
            exit(1);

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
