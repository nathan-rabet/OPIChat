#include <errno.h>
#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "room.h"

char *list_of_rooms(struct room *room)
{
    char *ret = strdup("");
    struct room *current = room;

    size_t size = 1;
    while (current)
    {
        size += strlen(current->name) + 1; // len + \n
        ret = xrealloc(ret, size, sizeof(char));
        strcat(ret, current->name);
        strcat(ret, "\n");

        current = current->next;
    }
    return ret;
}

struct send_pool *handle_list_rooms(struct message *msg, struct client *client)
{
    (void)msg;
    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = strdup("LIST-ROOMS");
    response->payload = list_of_rooms(rooms);

    response->payload_size = strlen(response->payload);

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
