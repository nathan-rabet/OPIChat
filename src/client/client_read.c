#include "client_read.h"
#include "message.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_BUFFER_SIZE 1

/**
 * @brief Read the server response
 *
 * @param server_socket The socket server
 * @return int 0 if successfull, 1 if not
 *
 */
void _read_from_server(int server_socket, char *message)
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
            strcat(message, buf);  
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
/*
void print_received_message(char *a)
{
    struct message *message = parse_message(a);
    if (message->status_code == RESPONSE_MESSAGE_CODE &&
strcmp(message->payload, "") != 0) fprintf(stdout, "< %s", message->payload);
    if(message->status_code == ERROR_MESSAGE_CODE)
        fprintf(stdout, "< %s", message->payload);
    if(message->status_code == NOTIFICATION_MESSAGE_CODE)
        if (strcmp(message->command, "SEND-DM") == 0 || strcmp(message->command,
"BROADCAST") == 0)
        {
            int i = 0;
            while (strcmp(message->command_parameters[i].key, "From") != 0
                   && i < message->nb_parameters)
                i++;
            fprintf(stdout, "From %s: %s\n",
                    (char *)message->command_parameters[i].value,
                    message->payload);
        }
}*/

void *read_thread(void *socket)
{
    int *s = socket;
    _read_from_server(*s, NULL);
    return s;
}
