#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "room.h"

struct send_pool *handle_send_room(struct message *msg, struct client *client)
{
    if (msg->nb_parameters != 1
        || strcmp(msg->command_parameters[0].key, "Room") != 0)
        return NULL;

    struct room *room = find_room(rooms, msg->command_parameters[0].value);

    if (errno == ROOM_ERROR_NOT_FOUND)
        return return_forged_error_message("SEND-ROOM", "Room not found\n",
                                           client->client_socket);

    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = strdup("SEND-ROOM");
    response->payload = strdup("");
    response->payload_size = 0;

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));
    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));

    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    size_t i = 0;
    while (i < room->nb_clients)
    {
        if (client->client_socket != room->clients_sockets[i])
        {
            struct client *client_to_send =
                find_client(clients, room->clients_sockets[i]);

            if (!client_to_send)
            {
                // Delete client from room
                room->clients_sockets[i] =
                    room->clients_sockets[room->nb_clients - 1];

                if (--room->nb_clients != 0)
                    room->clients_sockets = xrealloc(
                        room->clients_sockets, room->nb_clients, sizeof(int));
                continue;
            }

            // Send message to clients
            struct message *notification =
                init_message(NOTIFICATION_MESSAGE_CODE);
            notification->command = strdup("SEND-ROOM");
            notification->payload_size = msg->payload_size;
            notification->payload = strdup(msg->payload);
            notification->nb_parameters = 2;
            notification->command_parameters =
                xmalloc(2, sizeof(struct command_parameter));
            notification->command_parameters[0].key = strdup("From");
            notification->command_parameters[0].value = client_to_send->username
                ? strdup(client_to_send->username)
                : strdup("<Anonymous>");
            notification->command_parameters[1].key = strdup("Room");
            notification->command_parameters[1].value =
                strdup(msg->command_parameters[0].value);

            sp->nb_msg++;
            sp->msg = xrealloc(sp->msg, sp->nb_msg, sizeof(struct message));
            sp->msg[sp->nb_msg - 1] = notification;
            sp->clients_sockets[sp->nb_msg - 1] = room->clients_sockets[i];
        }

        i++;
    }

    return sp;
}
