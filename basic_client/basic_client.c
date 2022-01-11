#include "basic_client.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
 */
int _read_from_server(int server_socket)
{
    char buf[DEFAULT_BUFFER_SIZE] = { 0 };

    ssize_t read_len = 0;
    char hasLF = 0;
    while (!hasLF
           && (read_len = recv(server_socket, &buf, DEFAULT_BUFFER_SIZE, 0))
               != 0)
    {
        if (read_len == -1)
            return 1;
        // For the exercise, the server output is considered as valid
        for (int i = 0; i < read_len && !hasLF; i++)
            hasLF = (buf[i] == '\n');

        if (hasLF)
            printf("Server answered with: %s", buf);
    }
    return 0;
}

void communicate(int server_socket)
{
    char buf[DEFAULT_BUFFER_SIZE] = { 0 };

    ssize_t read_len = 0;
    ssize_t error = 0;
    fprintf(stderr, "Enter your message:\n");
    char hasLF = 0;
    while (!hasLF
           && (read_len = read(STDIN_FILENO, &buf, DEFAULT_BUFFER_SIZE)) != 0)
    {
        if (read_len == -1)
            exit(1);

        ssize_t i = 0;
        for (; i < read_len && !hasLF; i++)
            hasLF = (buf[i] == '\n');

        ssize_t send_len = 0;
        while ((error = send(server_socket, buf + send_len,
                             read_len - send_len - hasLF, MSG_MORE))
               != 0)
        {
            if (error == -1)
                exit(1);
            send_len += error;
        }

        if (hasLF)
        {
            error = send(server_socket, "\n", 1, MSG_EOR) < 1
                || _read_from_server(server_socket);
        }
    }
    close(server_socket);
    exit(error); // 1 ERROR, 0 OK
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SERVER_IP SERVER_PORT\n", argv[0]);
        exit(1);
    }

    int socket = prepare_socket(argv[1], argv[2]);

    communicate(socket);

    return 0;
}
