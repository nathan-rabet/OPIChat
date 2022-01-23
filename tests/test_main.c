#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "read_from_stdin.h"
#include "client.h"
#include "client_read.h"
#include "init_socket.h"
#include "safe_io.h"
#include "message.h"

int main(void)
{
    char client_request[] = "PING\n\n";
    struct message *server_response = NULL;
    struct message *server_request = NULL;

    // Fork a child process to send data and another to receive it
    pid_t pid = fork();
    if (pid == 0)
    {
        // Client process
        // Init a socket with the server
        usleep(100 * 1000);
        int s_clientfd = setup_client_socket("127.0.0.1", "7000");

        safe_write(STDIN_FILENO, client_request, sizeof(client_request));
        read_from_stdin(s_clientfd);
        server_response = safe_recv(s_clientfd, 0, false);

        close(s_clientfd);
        exit(0);
    }
    else
    {
        // Server process
        // Init a socket with the client
        int s_listenfd = setup_server_socket("127.0.0.1", "7000");
        int s_clientfd = accept(s_listenfd, NULL, NULL);

        server_request = safe_recv(s_clientfd, 0,true);

        close(s_listenfd);
        close(s_clientfd);

        free_message(server_request);
        waitpid(pid, NULL, 0);

        (void)server_response;
    }

    (void)server_response;

}
