#ifndef INIT_SOCKET_H_
#define INIT_SOCKET_H_

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG 16

/**
 * @brief Create a listening socket
 *
 * @param ip The ip to listen on
 * @param port The port to listen on
 * @return The created socket, exit on error
 */
int setup_server_socket(const char *ip, const char *port);
#endif
