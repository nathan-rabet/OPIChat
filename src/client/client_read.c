#include "client_read.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_BUFFER_SIZE 1


int create_and_connect(struct addrinfo *addrinfo)
{
    for (; addrinfo != NULL; addrinfo = addrinfo->ai_next)
    {
        int s = socket(addrinfo->ai_family, addrinfo->ai_socktype,
                       addrinfo->ai_protocol);
        if (s == -1)
            continue;

        int ok = 1;
        if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int)) == -1
            || ok != 1)
            break;

        if (connect(s, addrinfo->ai_addr, addrinfo->ai_addrlen) != -1)
            return s;
    }

    exit(1);
}

int prepare_socket(const char *ip, const char *port)
{
    struct addrinfo hints;
    struct addrinfo *addrinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip, port, &hints, &addrinfo) != 0)
        exit(1);

    int server_socket = create_and_connect(addrinfo);

    freeaddrinfo(addrinfo);
    return server_socket;
}

/**
 * @brief Read the server response
 *
 * @param server_socket The socket server
 * @return int 0 if successfull, 1 if not
 *
 */
/*
void _read_from_server(int server_socket)
{
    ssize_t buf_mult_factor = 1; // Number of times the buffer has been
                                 // reallocated (+ 1) // ! FIXME shitty comment
    char *buf = malloc(DEFAULT_BUFFER_SIZE);
    if (!buf)
    {
        fprintf(stderr, "Error while allocating memory\n");
        close(server_socket);
        exit(1);
    }

    ssize_t read_len = 0; // Number returned by read
    ssize_t msg_len = 0; // Total request length (can be higher than `read_len`
                         // in case of multiple loop)

    bool hasLF = 0; // Has any '\n' ?
    // While no '\n' and server is up
    // TODO SIGPIPE
    while ((!hasLF
            && (read_len =
                    recv(server_socket, buf + msg_len, DEFAULT_BUFFER_SIZE, 0))
                != 0))
    {
        // If error on reading from server
        if (read_len == -1)
        {
            free(buf);
            fprintf(stderr, "Error while reading server response\n");
            close(server_socket);
            exit(1);
        }

        // Determines real message length (must end with a '\n)
        for (; msg_len < buf_mult_factor * DEFAULT_BUFFER_SIZE && !hasLF;
             msg_len++)
            hasLF = (buf[msg_len] == '\n');

        // If '\n' detected, print message
        if (hasLF)
        {
            buf[msg_len] = '\0';
            printf("Server answered with: %s", buf);
        }
        else if (msg_len + DEFAULT_BUFFER_SIZE
                 > buf_mult_factor * DEFAULT_BUFFER_SIZE)
        {
            buf = realloc(buf, (++buf_mult_factor) * DEFAULT_BUFFER_SIZE);
            if (!buf)
            {
                fprintf(stderr, "Error while reallocating memory\n");
                close(server_socket);
                exit(1);
            }
        }
    }
    free(buf);
}

void communicate(int server_socket)
{
    ssize_t buf_mult_factor = 1; // buf[DEFAULT_BUFFER_SIZE * buf_mult_factor]

    char *buf = malloc(DEFAULT_BUFFER_SIZE);
    if (!buf)
    {
        fprintf(stderr, "Error while allocating memory\n");
        close(server_socket);
        exit(1);
    }

    // User entrypoint
    fprintf(stderr, "Enter your message:\n");

    ssize_t read_len; // Number returned by read
    ssize_t msg_len = 0; // Total request length (can be higher than
                         // `read_len` in case of multiple loop)

    bool hasLF = 0; // Has any '\n' ?
    while ((read_len = read(STDIN_FILENO, buf + msg_len, DEFAULT_BUFFER_SIZE))
           != 0)
    {
        // Has any STDIN reading error
        if (read_len == -1)
        {
            free(buf);

            fprintf(stderr, "Error while reading STDIN data\n");
            close(server_socket);
            exit(1);
        }

        // Determines real message length (must end with a '\n)
        for (; msg_len < buf_mult_factor * DEFAULT_BUFFER_SIZE && !hasLF;
             msg_len++)
            hasLF = (buf[msg_len] == '\n');

        // If '\n' detected
        if (hasLF)
        {
            // Sending buffer to server
            ssize_t error = 0;
            ssize_t send_len = 0;
            while ((error = send(server_socket, buf + send_len,
                                 msg_len - send_len, MSG_EOR))
                   > 0)
                send_len += error;

            // If any server sending error
            if (error == -1)
            {
                free(buf);
                fprintf(stderr, "Error while sending STDIN to the server\n");
                close(server_socket);
                exit(1);
            }

            // Read (and print) server response
            _read_from_server(server_socket);

            // Reset states for next message
            buf = realloc(buf, DEFAULT_BUFFER_SIZE); // Memory optimization
            buf_mult_factor = 1;
            hasLF = 0;
            msg_len = 0;
        }

        // If next loop can overflow the buffer
        else if (msg_len + DEFAULT_BUFFER_SIZE
                 > buf_mult_factor * DEFAULT_BUFFER_SIZE)
        {
            buf = realloc(buf, (++buf_mult_factor) * DEFAULT_BUFFER_SIZE);
            if (!buf)
            {
                fprintf(stderr, "Error while reallocating memory\n");
                close(server_socket);
                exit(1);
            }
        }
    }
    free(buf);
    close(server_socket);
}*/
/*
static void *communicate_thread(void *socket)
{
    int *s = socket;
    communicate(*s);
    return s;
}

static void *read_thread(void *socket)
{
    int *s = socket;
    _read_from_server(*s);
    return s;
}
*/
/*
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SERVER_IP SERVER_PORT\n", argv[0]);
        exit(1);
    }

    int socket = prepare_socket(argv[1], argv[2]);

    pthread_t *send_message = NULL;
    pthread_t *receive_from_server = NULL;
    int ret_send =  pthread_create (send_message, NULL, &communicate_thread, &socket);
    int ret_recv = pthread_create (receive_from_server, NULL, &read_thread, &socket);
    void *exited_with;
    pthread_join(ret_send, &exited_with);
    pthread_exit(NULL);
    printf("ret_recv = %d\n", ret_recv);
    printf("ret_send = %d\n", ret_send);
    return 0;
}
*/
