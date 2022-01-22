#ifndef SAFE_IO_H
#define SAFE_IO_H

#define DEFAULT_BUFFER_SIZE 2048

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "safe_io.h"
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
 * @param buf The reference to the buffer where the message will be stored.
 * @param flags the recv() flags.
 * @return The number of bytes received, -1 if an error occured.
 */
ssize_t safe_recv(int sockfd, void **buf, int flags);

/**
 * @brief Read a message from a file descriptor safely.
 *
 * @param fd The file descriptor of the client.
 * @param buf The reference to the buffer where the message will be stored.
 * @return The number of bytes read, -1 if an error occured.
 */
ssize_t safe_read(int fd, void **buf);

#endif /* SAFE_IO_H */
