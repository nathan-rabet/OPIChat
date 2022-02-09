#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "room.h"

struct send_pool *handle_leave_room(struct message *msg, struct client *client)
{
    if (msg->payload_size == 0)
        return NULL;

    struct room *room = find_room(rooms, msg->payload);

    if (errno == ROOM_ERROR_NOT_FOUND)
        return return_forged_error_message("JOIN-ROOM", "Room not found",
                                           client->client_socket);

    size_t i = 0;
    for (; i < room->nb_clients; i++)
        if (room->clients_sockets[i] == client->client_socket)
            break;

    if (i != room->nb_clients)
    {
        room->clients_sockets[i] = room->clients_sockets[room->nb_clients - 1];

        if (--room->nb_clients != 0)
            room->clients_sockets =
                xrealloc(room->clients_sockets, room->nb_clients, sizeof(int));
    }

    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = strdup("LEAVE-ROOM");
    response->payload = strdup("Room left");
    response->payload_size = strlen(response->payload);

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
