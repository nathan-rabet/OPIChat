#include "safe_io.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "xalloc.h"
#ifndef READ_BUFFER_SIZE // Usefull for debugging
#    define READ_BUFFER_SIZE 2048
#endif

int safe_send(int sockfd, const void *buf, size_t count, int flags)
{
    size_t offset = 0;

    while (offset < count)
    {
        ssize_t sent =
            send(sockfd, (char *)buf + offset, count - offset, flags);
        if (sent == -1)
            return -1;

        offset += sent;
    }

    return 0;
}

int safe_write(int fd, const void *buf, size_t count)
{
    size_t offset = 0;

    while (offset < count)
    {
        ssize_t sent = write(fd, (char *)buf + offset, count - offset);
        if (sent == -1)
            return -1;

        offset += sent;
    }

    return 0;
}

ssize_t safe_recv(int sockfd, void **buf, int flags)
{
    *buf = xrealloc(*buf, READ_BUFFER_SIZE, 1); // (Re)allocate the given buffer

    ssize_t nb_read = 0; // Total number of bytes read
    ssize_t read_len; // Number returned by read
    while ((read_len =
                recv(sockfd, (char *)buf + nb_read, READ_BUFFER_SIZE, flags))
           != 0)
    {
        // If any client reading error
        if (read_len == -1)
            return -1;
        nb_read += read_len;

        // If the buffer is full
        buf = xrealloc(buf, READ_BUFFER_SIZE + nb_read, 1);
    }

    return nb_read;
}

ssize_t safe_read(int fd, void **buf)
{
    *buf = xrealloc(*buf, READ_BUFFER_SIZE, 1); // (Re)allocate the given buffer

    ssize_t nb_read = 0; // Total number of bytes read
    ssize_t read_len; // Number returned by read
    while ((read_len = read(fd, (char *)*buf + nb_read, READ_BUFFER_SIZE)) != 0)
    {
        // If any client reading error
        if (read_len == -1)
            return -1;
        nb_read += read_len;

        // If the buffer is full
        *buf = xrealloc(*buf, READ_BUFFER_SIZE + nb_read, 1);
    }

    return nb_read;
}