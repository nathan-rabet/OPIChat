#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "init_socket.h"
#include "message.h"
#include "read_from_stdin.h"
#include "safe_io.h"

Test(read_from_stdin, request_ping, .disabled = true)
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
        sleep(10);
        int s_clientfd = setup_client_socket("127.0.0.1", "7000");

        FILE *fp = cr_get_redirected_stdin();
        write(fileno(fp), client_request, sizeof(client_request));
        read_from_stdin(s_clientfd);
        printf("CLIENT OK\n");
        server_response = safe_recv(s_clientfd, 0, true);

        printf("COMMAND %s\n", server_response->command);
        cr_assert_str_eq(server_response->command, "PING");
        cr_assert_str_eq(server_response->payload, "PONG");

        close(s_clientfd);
        exit(0);
    }
    else
    {
        // Server process
        // Init a socket with the client
        int s_listenfd = setup_server_socket("127.0.0.1", "7000");
        int s_clientfd = accept(s_listenfd, NULL, NULL);

        printf("Client FD ok\n");
        server_request = safe_recv(s_clientfd, 0, true);
        cr_expect_not_null(server_request);

        close(s_listenfd);
        close(s_clientfd);

        free_message(server_request);
        waitpid(pid, NULL, 0);
    }
}
