#ifndef INIT_SOCKET_H_
#define INIT_SOCKET_H_

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG 16

int server_socket;
char *ip;
char *port;

/**
 * @brief Create a listening socket
 *
 * @param ip The ip to listen on
 * @param port The port to listen on
 * @return The created socket, exit on error
 */
int setup_server_socket(const char *ip, const char *port);

/**
 * @brief Create a connection socket
 *
 * @param ip The ip to connect to
 * @param port The port to connect on
 * @return The created socket, -1 on error
 */
int setup_client_socket(const char *ip, const char *port);
#endif
