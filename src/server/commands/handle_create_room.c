#include <errno.h>
#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "room.h"
#include "xalloc.h"

struct send_pool *handle_create_room(struct message *msg, struct client *client)
{
    rooms = add_room(rooms, msg->payload, client->client_socket);
    switch (errno)
    {
    case ROOM_ERROR_BAD_NAME:
        return return_forged_error_message("CREATE_ROOM", "Bad room name\n",
                                           client->client_socket);

    case ROOM_ERROR_DUPPLICATE_NAME:
        return return_forged_error_message(
            "CREATE_ROOM", "Duplicate room name\n", client->client_socket);

    default:;
        struct message *response = init_message(RESPONSE_MESSAGE_CODE);
        response->payload_size = strlen("Room created\n");
        response->command = strdup("CREATE_ROOM");
        response->payload = strdup("Room created\n");
        struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

        sp->nb_msg = 1;
        sp->msg = xmalloc(1, sizeof(struct message *));
        sp->clients_sockets = xmalloc(1, sizeof(int));
        sp->msg[0] = response;
        sp->clients_sockets[0] = client->client_socket;

        return sp;
    }
}
