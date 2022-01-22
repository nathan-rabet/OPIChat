#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>

/**
 * @brief Contain all the information about all clients (linked list)
 *
 */
struct client
{
    int client_socket; // Socket FD of the client
    struct sockaddr_storage *client_sockaddr; // Client address

    ssize_t nb_read; // Number of bytes read (also size of the buffer)
    char *buffer; // Buffer containing all the data received by this client

    char *username; // Username of the client

    size_t nb_rooms; // Number of rooms the client is in
    char *rooms; // Rooms of the client // TODO : another struct ?

    struct client *next; // Next client for another client
};

/**
 * @brief Get the client IPv4/IPv6 address
 *
 * @warning This function returns dynamically allocated string
 * and thus must be freed after use
 *
 * @param client The client
 * @return char* The client IPv4/IPv6 address (x.x.x.x or x:x:x:x:x:x:x:x)
 */
char *get_client_ip(struct client *client);

/**
 * @brief Add a new client client to the linked list connection
 *
 * @param connection The client linked list with all the clients
 *
 * @param client_socket The client socket fd to add
 *
 * @param sockaddr The client socket address info (used to get the client ip)
 *
 * @param sockaddr_len The client socket address info length
 *
 * @return The client linked list with the element added
 *
 * Add the new client element with a head insert
 */
struct client *add_client(struct client *client, int client_socket,
                          struct sockaddr sockaddr, socklen_t sockaddr_len);

/**
 * @brief Remove the client client from the linked list connection
 *
 * @param connection The client linked list with all the clients
 *
 * @param client_socket The client socket fd to remove
 *
 * @return The client linked list with element removed
 *
 * Iterate over the linked list to find the right client and remove it
 */
struct client *remove_client(struct client *connection, int client_socket);

/**
 * @brief Find the client element where the socket is equal to client sock
 *
 * @param connection The client linked list with all the clients
 *
 * @param client_socket The client socket to find
 *
 * @return The client element of the specific client
 *
 * Iterate over the linked list until it finds the client. If the client
 * is not in the linked list returns NULL
 */
struct client *find_client(struct client *connection, int client_socket);

#endif /* CLIENT_H */
