#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

struct send_pool *handle_send_dm(struct message *msg, struct client *client)
{
    (void)client;

    if (msg->nb_parameters != 1
        || strcmp(msg->command_parameters[0].key, "User") != 0)
    {
        MISSING_PARAMETER_RETURN("SEND-DM")
    }

    // RESPONSE
    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = xmalloc(8, sizeof(char));
    strcpy(response->command, "SEND-DM");

    response->nb_parameters = 1;
    response->command_parameters = xmalloc(1, sizeof(struct command_parameter));

    response->command_parameters[0].key = strdup("User");
    response->command_parameters[0].value =
        strdup(msg->command_parameters[0].value);

    // response->command_parameters[0].key = strdup("From");
    // response->command_parameters[0].value =
    //     strdup(find_client(clients, client->client_socket)->username);

    // NOTIFICATION
    struct client *notified_client =
        find_client_by_username(clients, msg->command_parameters[0].value);

    if (!notified_client || !notified_client->username)
    {
        struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

        free_message(response);
        struct message *error_message = init_message(ERROR_MESSAGE_CODE);
        error_message->payload = strdup("User not found\n");
        error_message->command = strdup("SEND-DM");
        error_message->payload_size = strlen(error_message->payload);

        sp->nb_msg = 1;
        sp->msg = xmalloc(1, sizeof(struct message));
        sp->clients_sockets = xmalloc(1, sizeof(int));
        *sp->msg = error_message;
        *sp->clients_sockets = client->client_socket;

        return sp;
    }
    struct message *notification = init_message(NOTIFICATION_MESSAGE_CODE);
    notification->command = strdup("SEND-DM");
    notification->payload_size = msg->payload_size;
    notification->payload = strdup(msg->payload);
    notification->nb_parameters = 2;

    notification->command_parameters =
        xmalloc(2, sizeof(struct command_parameter));
    notification->command_parameters[0].key = strdup("User");
    notification->command_parameters[0].value =
        strdup(msg->command_parameters[0].value);
    notification->command_parameters[1].key = strdup("From");
    notification->command_parameters[1].value =
        strdup(notified_client->username);

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 2;
    sp->msg = xmalloc(2, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->msg[1] = notification;
    sp->clients_sockets[0] = client->client_socket;
    sp->clients_sockets[1] = notified_client->client_socket;

    return sp;
}
