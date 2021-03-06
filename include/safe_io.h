#ifndef SAFE_IO_H
#define SAFE_IO_H

#define DEFAULT_BUFFER_SIZE 2048
#define RECV_TIMEOUT 5

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "xalloc.h"

/**
 * @brief Send a message to a client safely.
 *
 * @param sockfd The file descriptor of the client.
 * @param buf The buffer containing the message.
 * @param count The number of bytes to send.
 * @param flags the send() flags.
 * @return 0 if the message has been sent, -1 otherwise.
 */
int safe_send(int sockfd, const void *buf, size_t count, int flags);

/**
 * @brief Write a message to a file descriptor safely.
 *
 * @param fd The file descriptor of the client.
 * @param buf The buffer containing the message.
 * @param count The number of bytes to write.
 * @return 0 if the message has been sent, -1 otherwise.
 */
int safe_write(int fd, const void *buf, size_t count);

/**
 * @brief Receive a message from a client safely.
 *
 * @param sockfd The file descriptor of the client.
 * @param flags the recv() flags.
 * @param mustTimeout If true, the function will exit after RECV_TIMEOUT
 * seconds, otherwise it will wait for the message to be received forever.
 * @return The message received, NULL if an error occured.
 */
struct message *safe_recv(int sockfd, int flags, bool mustTimeout);

/**
 * @brief Read a message from a file descriptor safely.
 *
 * @warning MUST NOT BE USED FOR SOCKETS.
 *
 * @param fd The file descriptor of the client.
 * @param buf The reference to the buffer where the message will be stored.
 * @return The number of bytes read, -1 if an error occured.
 */
ssize_t safe_read(int fd, void **buf);
#endif /* SAFE_IO_H */
