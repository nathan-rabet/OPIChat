#include <errno.h>
#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "room.h"

struct send_pool *handle_delete_room(struct message *msg, struct client *client)
{
    // TODO
    rooms = remove_room(rooms, msg->payload, client->client_socket);

    switch (errno)
    {
    case ROOM_ERROR_NOT_FOUND:
        return return_forged_error_message("DELETE-ROOM", "Room not \n",
                                           client->client_socket);
    case ROOM_ERROR_UNAUTHORIZED:
        return return_forged_error_message("DELETE-ROOM", "Unauthorized\n",
                                           client->client_socket);
    default:; // ROOM_ERROR_NONE
        struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

        struct message *response = init_message(RESPONSE_MESSAGE_CODE);
        response->command = strdup("DELETE-ROOM");
        response->payload = strdup("Room deleted\n");
        response->payload_size = strlen(response->payload);

        sp->nb_msg = 1;
        sp->msg = xmalloc(1, sizeof(struct message *));
        sp->clients_sockets = xmalloc(1, sizeof(int));
        sp->msg[0] = response;
        sp->clients_sockets[0] = client->client_socket;

        return sp;
    }
}
