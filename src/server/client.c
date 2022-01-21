#include "client.h"

#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"
#include "xalloc.h"

char *get_client_ip(struct client *client)
{
    struct sockaddr_storage *addr =
        (struct sockaddr_storage *)&client->client_sockaddr;

    if (addr->ss_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)addr;

        char *ipv4_str = inet_ntoa(ipv4->sin_addr);
        return strdup(ipv4_str);
    }
    else
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)addr;
        char ipv6_str[INET6_ADDRSTRLEN];
        return strdup(
            inet_ntop(AF_INET6, &ipv6->sin6_addr, ipv6_str, INET6_ADDRSTRLEN));
    }
}

struct client *add_client(struct client *client, int client_socket,
                          struct sockaddr sockaddr, socklen_t sockaddr_len)
{
    struct client *new_connection = xcalloc(1, sizeof(struct client));

    new_connection->client_socket = client_socket;
    new_connection->next = client;
    new_connection->client_sockaddr =
        xcalloc(1, sizeof(struct sockaddr_storage));
    memcpy(new_connection->client_sockaddr, &sockaddr, sockaddr_len);

    return new_connection;
}

/**
 * @brief Free the memory allocated for a client
 *
 * @param client The client to free
 *
 */
static void __free_client(struct client *client)
{
    if (close(client->client_socket) == -1)
        write_error("Failed to close socket for client %s",
                    get_client_ip(client));
    free(client->buffer);
    free(client->client_sockaddr);
    free(client);
}

struct client *remove_client(struct client *client, int client_socket)
{
    if (client)
    {
        if (client->client_socket == client_socket)
        {
            struct client *client_connection = client->next;
            __free_client(client);
            return client_connection;
        }

        struct client *tmp = client;
        while (tmp->next)
        {
            if (tmp->next->client_socket == client_socket)
            {
                __free_client(tmp->next);
                break;
            }
            tmp = tmp->next;
        }
    }
    return NULL;
}

struct client *find_client(struct client *client, int client_socket)
{
    while (client != NULL && client->client_socket != client_socket)
        client = client->next;

    return client;
}
