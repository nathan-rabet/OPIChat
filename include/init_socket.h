#ifndef INIT_SOCKET_H_
#define INIT_SOCKET_H_

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG 16

/**
 * @brief Init the socket system and bind it
 *
 * @param addrinfo The address info to use
 * @return The created socket, exit on error
 */
int setup_socket_basis(struct addrinfo *addrinfo);

/**
 * @brief Create a listening socket
 *
 * @param ip The ip to listen on
 * @param port The port to listen on
 * @return The created socket, exit on error
 */
int setup_socket(const char *ip, const char *port);
#endif
