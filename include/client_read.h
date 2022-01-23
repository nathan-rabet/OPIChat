#ifndef CLIENT_READ_H
#define CLIENT_READ_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

void *read_thread(void *socket);

#endif /* CLIENT_READ_H */
