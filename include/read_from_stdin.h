#ifndef READ_FROM_STDIN_H
#define READ_FROM_STDIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

void *read_from_stdin_thread(void *none);

/**
 * @brief Read from stdin and send the message to the server
 *
 * @param server_socket The socket to send the message to
 */
void read_from_stdin(int server_socket);

#endif /* READ_FROM_STDIN_H */

