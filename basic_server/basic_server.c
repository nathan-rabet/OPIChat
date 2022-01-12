#include "basic_server.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CONNECTION_NB 5

int create_and_bind(struct addrinfo *addrinfo)
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

        if (bind(s, addrinfo->ai_addr, addrinfo->ai_addrlen) != -1)
            return s;
    }

    exit(1);
}

int prepare_socket(const char *ip, const char *port)
{
    struct addrinfo hints = { 0 };
    struct addrinfo *addrinfo;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip, port, &hints, &addrinfo) != 0)
        exit(1);

    int server_socket = create_and_bind(addrinfo);

    freeaddrinfo(addrinfo);

    if (listen(server_socket, MAX_CONNECTION_NB) == -1)
        exit(1);

    return server_socket;
}

int accept_client(int socket)
{
    struct sockaddr client_addr;
    socklen_t addr_len;

    int client_socket = accept(socket, &client_addr, &addr_len);

    if (client_socket >= 0)
        printf("Client connected\n");

    return client_socket;
}

void communicate(int client_socket)
{
    ssize_t buf_mult_factor = 1; // buf[DEFAULT_BUFFER_SIZE * buf_mult_factor]

    char *buf = malloc(DEFAULT_BUFFER_SIZE);
    if (!buf)
    {
        fprintf(stderr, "Error while allocating memory\n");
        close(client_socket);
        exit(1);
    }

    ssize_t read_len; // Number returned by read
    ssize_t msg_len = 0; // Total request length (can be higher than
                         // `read_len` in case of multiple loop)

    bool hasLF = 0; // Has any '\n' ?
    while ((read_len = read(client_socket, buf + msg_len, DEFAULT_BUFFER_SIZE))
           != 0)
    {
        // If any client reading error
        if (read_len == -1)
        {
            fprintf(stderr, "Error while reading client data\n");
            close(client_socket);
            free(buf);
            exit(1);
        }

        // Determines real message length (must end with a '\n)
        for (; msg_len < buf_mult_factor * DEFAULT_BUFFER_SIZE && !hasLF;
             msg_len++)
            hasLF = (buf[msg_len] == '\n');

        // If '\n' detected
        if (hasLF)
        {
            buf[msg_len] = '\0';
            printf("Received Body: %s", buf);

            // Send message back to client
            ssize_t error = 0;
            ssize_t send_len = 0;
            while ((error = send(client_socket, buf + send_len,
                                 msg_len - send_len, MSG_EOR))
                   > 0)
                send_len += error;

            // If any client sending error
            if (error == -1)
            {
                fprintf(stderr, "Error while sending back data\n");
                close(client_socket);
                exit(1);
            }

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
                free(buf);
                fprintf(stderr, "Error while reallocating memory\n");
                close(client_socket);
                exit(1);
            }
        }
    }
    printf("Client disconnected\n");
    free(buf);
    close(client_socket);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        exit(1);
    }

    int server_listen_socket = prepare_socket(argv[1], argv[2]);

    while (1)
    {
        int client_sock = accept_client(server_listen_socket);
        communicate(client_sock);
    }

    return 0;
}
