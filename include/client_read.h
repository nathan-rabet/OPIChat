#ifndef CLIENT_READ_H
#define CLIENT_READ_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

int prepare_socket(const char *ip, const char *port);
int create_and_connect(struct addrinfo *addrinfo);
void _read_from_server(int server_socket);

/**
 * \brief Handle communication with the server
 *
 * \param server_socket: server socket
 *
 * Read user message from STDIN and send it to the server
 *
 * Step 2: This function sends small messages to the server
 *
 * Step 3: In addition to step 2 behavior, this function receives the server
 * response on small messages
 *
 * Step 4: The function must now handle long messages
 */
void communicate(int server_socket);

#endif /* CLIENT_READ_H */
