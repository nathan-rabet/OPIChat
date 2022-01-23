#include "safe_io.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "message.h"
#include "xalloc.h"

#ifndef READ_BUFFER_SIZE // Usefull for debugging
#    define READ_BUFFER_SIZE 2048
#endif

int safe_send(int sockfd, const void *buf, size_t count, int flags)
{
    size_t offset = 0;

    while (offset < count)
    {
        ssize_t sent = send(sockfd, (char *)buf + offset, count - offset,
                            flags | MSG_NOSIGNAL);
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

struct safe_recv_data
{
    int sockfd;
    int flags;
};

static void *_thread_safe_recv(void *arg)
{
    struct safe_recv_data *data = (struct safe_recv_data *)arg;

    int sockfd = data->sockfd;
    int flags = data->flags;

    char *buf = xmalloc(READ_BUFFER_SIZE, 1); // (Re)allocate the given buffer

    ssize_t nb_read = 0; // Total number of bytes read
    ssize_t read_len; // Number returned by read
    while ((read_len = recv(sockfd, buf + nb_read, READ_BUFFER_SIZE - 1, flags))
           != 0)
    {
        // If any client reading error
        if (read_len == -1)
            return NULL;

        nb_read += read_len;

        buf[nb_read] = '\0';
        struct message *message = NULL;
        if ((message = parse_message(buf)) != NULL)
            return (void *)message;

        // If the buffer is full
        buf = xrealloc(buf, READ_BUFFER_SIZE + nb_read, 1);
    }

    return NULL;
}

struct message *safe_recv(int sockfd, int flags)
{
    // Create thread and kill it if it takes too long
    pthread_t thread;
    struct safe_recv_data *data = xmalloc(1, sizeof(struct safe_recv_data));
    data->sockfd = sockfd;
    data->flags = flags;
    pthread_create(&thread, NULL, _thread_safe_recv, (void *)data);

    struct message *returned_message = NULL;

    struct timespec timeout;
    timeout.tv_sec = time(NULL) + RECV_TIMEOUT;
    timeout.tv_nsec = 0;

    pthread_timedjoin_np(thread, (void *)&returned_message, &timeout);
    pthread_cancel(thread);
    pthread_join(thread, NULL);

    free(data);
    return returned_message;
}
