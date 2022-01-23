#ifndef CLIENT_READ_H
#define CLIENT_READ_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

/**
 * @brief Create a connection socket
 *
 * @param ip The ip to connect to
 * @param port The port to connect on
 * @return The created socket, exit on error
 */
int setup_client_socket(const char *ip, const char *port);
int create_and_connect(struct addrinfo *addrinfo);
void *read_thread(void *socket);

#endif /* CLIENT_READ_H */
