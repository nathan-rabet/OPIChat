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
    new_connection->ip = get_client_ip(new_connection);

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
    free(client->client_sockaddr);
    free(client->ip);
    free(client);
}

struct client *remove_client(struct client *client, int client_socket)
{
    struct client *prev = NULL;
    struct client *parser = client;
    while (parser)
    {
        if (parser->client_socket == client_socket)
        {
            if (prev) // If the client is not the first one
                prev->next = parser->next;
            else // If the client is the first one
                client = parser->next;

            __free_client(parser);
            break;
        }
        prev = parser;
        parser = parser->next;
    }
    return client;
}

struct client *find_client(struct client *client, int client_socket)
{
    while (client != NULL && client->client_socket != client_socket)
        client = client->next;

    return client;
}

struct client *find_client_by_username(struct client *client, char *username)
{
    while (client != NULL
           && (!client->username || strcmp(client->username, username) != 0))
        client = client->next;

    return client;
}
