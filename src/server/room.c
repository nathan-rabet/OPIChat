#include "room.h"

#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "commands.h"
#include "logger.h"
#include "message.h"
#include "xalloc.h"

struct send_pool *return_forged_error_message(char *command, char *payload,
                                              int send_socket)
{
    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));
    struct message *error_message = init_message(ERROR_MESSAGE_CODE);
    error_message->command = strdup(command);
    error_message->payload = strdup(payload);
    error_message->payload_size = strlen(error_message->payload);
    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    *sp->msg = error_message;
    *sp->clients_sockets = send_socket;
    return sp;
}

struct room *add_room(struct room *room, const char *room_name,
                      int owner_socket)
{
    // Verify if room_name is alphanumeric
    if (strspn(
            room_name,
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ")
        != strlen(room_name))
    {
        errno = ROOM_ERROR_BAD_NAME;
        return room;
    }

    // Verify if the room already exists
    if (find_room(room, room_name) != NULL)
    {
        errno = ROOM_ERROR_DUPPLICATE_NAME;
        return room;
    }

    struct room *new_room = xcalloc(1, sizeof(struct room));

    new_room->owner_socket = owner_socket;
    new_room->name = strdup(room_name);
    new_room->next = room;

    errno = ROOM_ERROR_NONE;
    return new_room;
}

/**
 * @brief Free the memory allocated for a room
 *
 * @param room The room to free
 *
 */
static void __free_room(struct room *room)
{
    free(room->clients_sockets);
    free(room->name);
    free(room);
}

struct room *remove_room(struct room *room, const char *room_name,
                         int demander_socket)
{
    struct room *prev = NULL;
    struct room *parser = room;
    while (parser)
    {
        if (strcmp(parser->name, room_name) == 0)
        {
            // Check if client_demander_socket is authorized to remove the room
            if (parser->owner_socket != demander_socket)
            {
                errno = ROOM_ERROR_UNAUTHORIZED;
                return room;
            }

            if (prev) // If the room is not the first one
                prev->next = parser->next;
            else // If the room is the first one
                room = parser->next;

            __free_room(parser);
            errno = ROOM_ERROR_NONE;
            return room;
        }
        prev = parser;
        parser = parser->next;
    }

    // No room found
    errno = ROOM_ERROR_NOT_FOUND;
    return room;
}

struct room *find_room(struct room *room, const char *room_name)
{
    while (room != NULL)
    {
        if (strcmp(room->name, room_name) == 0)
        {
            errno = ROOM_ERROR_NONE;
            return room;
        }
        room = room->next;
    }

    errno = ROOM_ERROR_NOT_FOUND;
    return NULL;
}
