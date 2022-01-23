#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"
#include "commands.h"
#include "logger.h"
#include "message.h"
#include "safe_io.h"
#include "xalloc.h"

extern struct client *clients;
extern struct room *rooms;

void send_invalid_message_error(int client_socket)
{
    // Send an error message to the client
    struct message error_response = { 0 };

    error_response.status_code = ERROR_MESSAGE_CODE;
    error_response.command = "INVALID";
    error_response.payload = "Bad request";

    char *encoded_response = compose_message(&error_response);

    if (safe_send(client_socket, encoded_response, strlen(encoded_response),
                  MSG_EOR)
        == -1)
        write_warning("Failed to send error message to client %d",
                      client_socket);
}

struct client *accept_client(int epoll_instance, int server_socket)
{
    struct sockaddr client_sockaddr;
    socklen_t sockaddr_len;

    int client_socket = accept(server_socket, &client_sockaddr, &sockaddr_len);

    if (client_socket == -1)
        return clients;

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = client_socket;
    if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, client_socket, &event) == -1)
        write_error("Impossible to add the client to the epoll instance");

    return add_client(clients, client_socket, client_sockaddr, sockaddr_len);
}

struct client *delete_epoll_client(int epoll_instance, int client_socket)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = client_socket;
    if (epoll_ctl(epoll_instance, EPOLL_CTL_DEL, client_socket, &event) == -1)
        write_error("Impossible to delete the client with socket %d from the "
                    "epoll instance",
                    client_socket);

    return remove_client(clients, client_socket);
}

struct client *communicate(int epoll_instance, int client_socket)
{
    struct client *client = find_client(clients, client_socket);

    client->nb_read = safe_recv(client_socket, (void *)&client->buffer, 0);
    if (client->nb_read == -1)
    {
        write_warning("Impossible to read from the client %s with socket %d",
                      get_client_ip(client), client_socket);
        return delete_epoll_client(epoll_instance, client_socket);
    }

    struct message *m = parse_message(client->buffer);
    if (!m)
    {
        send_invalid_message_error(client_socket);
        return clients;
    }

    else
    {
        struct message *response = NULL;

        // Core features
        if (strcmp(m->command, "PING"))
            response = handle_ping(m, client);

        else if (strcmp(m->command, "LOGIN"))
            response = handle_login(m, client);

        else if (strcmp(m->command, "LIST-USERS"))
            response = handle_list_users(m, client);

        else if (strcmp(m->command, "SEND-DM"))
            response = handle_send_dm(m, client);

        else if (strcmp(m->command, "BROADCAST"))
            response = handle_broadcast(m, client);

        // Additionnal features
        else if (strcmp(m->command, "CREATE-ROOM"))
            response = handle_create_room(m, client);

        else if (strcmp(m->command, "LIST-ROOMS"))
            response = handle_list_rooms(m);

        else if (strcmp(m->command, "JOIN-ROOM"))
            response = handle_join_room(m, client);

        else if (strcmp(m->command, "LEAVE-ROOM"))
            response = handle_leave_room(m, client);

        else if (strcmp(m->command, "SEND-ROOM"))
            response = handle_send_room(m, client);

        else if (strcmp(m->command, "DELETE-ROOM"))
            response = handle_delete_room(m, client);

        else if (strcmp(m->command, "PROFILE"))
            response = handle_profile(m, client);

        else
        {
            write_warning("Client %s with socket %d sent a message with an "
                          "unsupported command",
                          get_client_ip(client), client_socket);
            send_invalid_message_error(client_socket);
        }

        if (response)
        {
            char *encoded_response = compose_message(response);
            if (safe_send(client_socket, encoded_response,
                          strlen(encoded_response), MSG_EOR)
                == -1)
                write_warning("Failed to send a response to client %d",
                              client_socket);
            free(encoded_response);
            free_partial_message(response);
        }
    }

    return clients;
}
