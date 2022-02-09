#ifndef ROOM_H
#define ROOM_H

#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"

/**
 * @brief Contain all the information about all rooms (linked list)
 *
 */
struct room *rooms;

enum room_errors
{
    ROOM_ERROR_NONE,
    ROOM_ERROR_BAD_NAME,
    ROOM_ERROR_DUPPLICATE_NAME,
    ROOM_ERROR_NOT_FOUND,
    ROOM_ERROR_UNAUTHORIZED
};

struct room
{
    char *name; // Name of the room
    int owner_socket; // Socket FD of the room owner

    size_t nb_clients; // Number of clients in the room
    int *clients_sockets; // Clients in the room

    struct room *next; // Next room for another room
};

/**
 * @brief Add a new room room to the linked list room
 *
 * @param room The room linked list with all the rooms
 * @param room_name The name of the room to add
 * must be alphanumeric, else errno is set to ROOM_ERROR_BAD_NAME
 * must be unique (not already in linked list), else errno is set to
 * ROOM_ERROR_DUPPLICATE_NAME
 * @param owner_socket The socket FD of the room owner
 *
 * @return The room linked list with the element added
 */
struct room *add_room(struct room *room, const char *room_name,
                      int owner_socket);

/**
 * @brief Remove the room room from the linked list room
 *
 * @param room The room linked list with all the rooms
 * @param room_name The room name to remove,
 * must be included in the linked list else errno is set to ROOM_ERROR_NOT_FOUND
 * @param demander_socket The socket FD of the client
 * who wants to remove the room, must be the owner of the room else errno is set
 * to ROOM_ERROR_UNAUTHORIZED
 *
 * @return The room linked list with the element removed
 */
struct room *remove_room(struct room *room, const char *room_name,
                         int demander_socket);

/**
 * @brief Find the room element where the socket is equal to room sock
 *
 * @param room The room linked list with all the rooms
 * @param room_name The room name to find
 *
 * @return The room element of the specific room, if not found errno is set to
 * ROOM_ERROR_NOT_FOUND
 */
struct room *find_room(struct room *room, const char *room_name);

struct send_pool *return_forged_error_message(char *command, char *payload,
                                              int send_socket);

#endif /* ROOM_H */
