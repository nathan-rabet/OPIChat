#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

struct send_pool *handle_broadcast(struct message *msg, struct client *client)
{
    (void) client;
    if (msg->nb_parameters != 0)
        return NULL;

    char *client_name = client->username ? client->username : "<Anonymous>";

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = strdup("BROADCAST");
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->nb_msg = 1;
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    for (struct client *cur = clients; cur; cur = cur->next)
    {
        if (cur->client_socket != client->client_socket)
        {
            struct message *notification =
                init_message(NOTIFICATION_MESSAGE_CODE);
            notification->command = strdup("BROADCAST");
            notification->payload_size = msg->payload_size;
            notification->payload = strdup(msg->payload);
            notification->nb_parameters = 1;
            notification->command_parameters =
                xmalloc(1, sizeof(struct command_parameter));
            notification->command_parameters->key = strdup("From");
            notification->command_parameters->value = strdup(client_name);

            sp->nb_msg++;
            sp->msg = xrealloc(sp->msg, sp->nb_msg, sizeof(struct message));
            sp->msg[sp->nb_msg - 1] = notification;
            sp->clients_sockets[sp->nb_msg - 1] = cur->client_socket;
        }
    }

    return sp;
}
