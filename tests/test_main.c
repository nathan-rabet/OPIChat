#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "client_read.h"
#include "init_socket.h"
#include "message.h"
#include "safe_io.h"


int main(void)
{
    struct message *m = NULL;
    char buf_in[] = "4\n0\nSEND-DM\nUser=acu\n\n2022";

    // Fork a child process to send data and another to receive it
    pid_t pid = fork();
    if (pid == 0)
    {
        // H4ck3r man
        // Init a socket with the server
        usleep(100 * 1000);
        int s_clientfd = setup_client_socket("127.0.0.1", "8082");

        send(s_clientfd, buf_in, 10, MSG_MORE);
        sleep(3);
        send(s_clientfd, buf_in + 10, 10, MSG_MORE);
        sleep(7);
        send(s_clientfd, buf_in + 20, 5, MSG_MORE);
        sleep(20);
        send(s_clientfd, buf_in + 25, 2, MSG_EOR);
        close(s_clientfd);
        exit(0);
    }
    else
    {
        // Server process
        // Init a socket with the client
        int s_listenfd = setup_server_socket("127.0.0.1", "8082");
        int s_clientfd = accept(s_listenfd, NULL, NULL);
}
