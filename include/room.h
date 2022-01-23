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
    struct client *clients; // Clients in the room

    struct room *next; // Next room for another room
};

/**
 * @brief Add a new room room to the linked list room
 *
 * @param room The room linked list with all the rooms
 * @param room_name The name of the room to add
 * @param owner_socket The socket FD of the room owner
 *
 * @return The room linked list with the element added
 *
 * Add the new room element with a head insert
 */
struct room *add_room(struct room *room, const char *room_name,
                      int owner_socket);

/**
 * @brief Remove the room room from the linked list room
 *
 * @param room The room linked list with all the rooms
 * @param room_name The room name to remove
 * @param demander_socket The socket FD of the client
 * who wants to remove the room
 *
 * @return The room linked list with element removed
 *
 * Iterate over the linked list to find the right room and remove it
 */
struct room *remove_room(struct room *room, const char *room_name,
                         int demander_socket);

/**
 * @brief Find the room element where the socket is equal to room sock
 *
 * @param room The room linked list with all the rooms
 * @param room_name The room name to find
 *
 * @return The room element of the specific room
 *
 * Iterate over the linked list until it finds the room. If the room
 * is not in the linked list returns NULL
 */
struct room *find_room(struct room *room, const char *room_name);

#endif /* ROOM_H */
