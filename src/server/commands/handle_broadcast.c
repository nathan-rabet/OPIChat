#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

struct send_pool *handle_broadcast(struct message *msg, struct client *client)
{
    (void) client;
    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = xmalloc(10, sizeof(char));
    strcpy(response->command, "BROADCAST");

    struct message *notification = init_message(NOTIFICATION_MESSAGE_CODE);
    notification->payload_size = msg->payload_size;
    response->command = xmalloc(5, sizeof(char));
    strcpy(response->command, "BROADCAST");
    notification->payload = xmalloc(strlen(msg->payload) + 1, sizeof(char));
    notification->payload_size = msg->payload_size;
    notification->nb_parameters = msg->nb_parameters;
    if(msg->nb_parameters != 0)
        notification->command_parameters = xmalloc(notification->nb_parameters,sizeof(struct command_parameter));
    for(uint64_t i = 0; i < msg->nb_parameters; i++)
    {
        notification->command_parameters[i].key = xmalloc(strlen(msg->command_parameters[i].key) + 1, sizeof(char));
        notification->command_parameters[i].value = xmalloc(strlen(msg->command_parameters[i].value) + 1, sizeof(char));
        strcpy(notification->command_parameters[i].key, msg->command_parameters[i].key);
        strcpy(notification->command_parameters[i].value, msg->command_parameters[i].value);
    }
    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
