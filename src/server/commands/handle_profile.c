#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
struct send_pool *handle_profile(struct message *msg, struct client *client)
{
    // Send a response to the client with all the information about the user

    if (msg->payload_size != 0)
        return NULL;

    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = strdup("PROFILE");

    if (!client->username)
        client->username = strdup("Anonymous");

    // Add the username
    response->payload_size +=
        strlen("Username: ") + strlen(client->username) + 1;
    response->payload = xcalloc(response->payload_size + 1, sizeof(char));
    strcat(response->payload, "Username: ");
    strcat(response->payload, client->username);
    strcat(response->payload, "\n");

    // Add the IP address
    response->payload_size += strlen("IP: ") + strlen(client->ip) + 1;
    response->payload =
        xrealloc(response->payload, response->payload_size + 1, sizeof(char));
    strcat(response->payload, "IP: ");
    strcat(response->payload, client->ip);
    strcat(response->payload, "\n");

    // Adding all the rooms where the user is

    if (client->nb_rooms > 0)
    {
        response->payload_size += strlen("Rooms:") + 1;
        response->payload = xrealloc(response->payload,
                                     response->payload_size + 1, sizeof(char));
        strcat(response->payload, "Rooms:\n");
        for (size_t i = 0; i < client->nb_rooms; i++)
        {
            response->payload_size += strlen(client->rooms[i]) + 1;
            response->payload = xrealloc(
                response->payload, response->payload_size + 1, sizeof(char));
            strcat(response->payload, client->rooms[i]);
            strcat(response->payload, "\n");
        }
    }
    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));
    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
