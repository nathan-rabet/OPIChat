#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "room.h"

struct send_pool *handle_join_room(struct message *msg, struct client *client)
{
    if (msg->payload_size == 0)
        return NULL;

    struct room *room = find_room(rooms, msg->payload);

    if (errno == ROOM_ERROR_NOT_FOUND)
        return return_forged_error_message("JOIN-ROOM", "Room not found\n",
                                           client->client_socket);

    bool isAlreadyIn = false;
    for (size_t i = 0; !isAlreadyIn && i < room->nb_clients; i++)
        isAlreadyIn = room->clients_sockets[i] == client->client_socket;

    if (!isAlreadyIn)
    {
        room->clients_sockets =
            xrealloc(room->clients_sockets, ++room->nb_clients, sizeof(int));
        room->clients_sockets[room->nb_clients - 1] = client->client_socket;

        // Add room to client
        client->rooms =
            xrealloc(client->rooms, ++client->nb_rooms, sizeof(char *));
        client->rooms[client->nb_rooms - 1] = strdup(msg->payload);
    }

    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = strdup("JOIN-ROOM");
    response->payload = strdup("Room joined\n");
    response->payload_size = strlen(response->payload);

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
