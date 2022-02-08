#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

char *list_of_users(struct client *client)
{
    char *ret = strdup("");
    struct client *current = client;

    size_t size = 1;
    while (current)
    {
        if (current->username && strcmp(current->username, "") != 0)
        {
            size += strlen(current->username) + 1; // len + \n
            ret = xrealloc(ret, size, sizeof(char));
            strcat(ret, current->username);
            strcat(ret, "\n");
        }

        current = current->next;
    }
    return ret;
}
struct send_pool *handle_list_users(struct message *msg, struct client *client)
{
    (void) msg;
    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = strdup("LIST-USERS");
    response->payload = list_of_users(client);

    response->payload_size = strlen(response->payload);

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
