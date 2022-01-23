#ifndef EPOLL_SERVER_H_
#define EPOLL_SERVER_H_

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"

int epoll_instance;

/**
 * @brief The length of the event array, must be greater than zero
 */
#define MAX_EVENTS 64

/**
 * @brief Accept a new client and add it to the `struct client`
 *
 * @warning The gloal `struct client` may be modified
 *
 * @param server_socket Listening socket
 *
 * @return The connection struct with the new client added
 */
void accept_client(int server_socket);

/**
 * @brief Establish recv/send communication with the client
 *
 * @warning The gloal `struct client` may be modified
 *
 * @param client_socket The socket of the client
 */
void communicate(int client_socket);

#endif /* EPOLL_SERVER_H_ */

