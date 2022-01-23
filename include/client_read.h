#ifndef CLIENT_READ_H
#define CLIENT_READ_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

int setup_client_socket(const char *ip, const char *port);
void *read_thread(void *socket);

#endif /* CLIENT_READ_H */
