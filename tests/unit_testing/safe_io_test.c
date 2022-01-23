#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "client_read.h"
#include "init_socket.h"
#include "logger.h"
#include "message.h"
#include "safe_io.h"

Test(safe_io, safe_write, .init = cr_redirect_stdout)
{
    char buf_in[1024] = { 'H', 'e', 'l', 'l', 'o', ' ',
                          'W', 'o', 'r', 'l', 'd', '\0' };

    // Write test
    int isOk = safe_write(STDOUT_FILENO, buf_in, strlen(buf_in)) == 0;
    cr_assert(isOk, "safe_write failed");
    cr_stdout_match_str("Hello World");
}

Test(safe_io, safe_read)
{
    char *buf_out = NULL;
    char buf_in[] = { 'H', 'e', 'l', 'l', 'o', ' ',
                      'W', 'o', 'r', 'l', 'd', '\0' };

    FILE *fp = fopen("safe_io_test.txt", "w+");
    cr_assert(fp != NULL, "safe_read failed");

    // Read test
    safe_write(fileno(fp), buf_in, sizeof(buf_in));
    fseek(fp, 0, SEEK_SET); // Set to the beginning of the file for reading

    int nb_read = safe_read(fileno(fp), (void **)&buf_out);
    cr_expect_eq(nb_read, sizeof(buf_in), "Actual number of bytes read: %d",
                 nb_read);
    cr_expect_str_eq(buf_out, buf_in);

    free(buf_out);
    fclose(fp);
    unlink("safe_io_test.txt");
}

Test(safe_io, safe_send_recv)
{
    struct message *m = NULL;
    char buf_in[] = "4\n0\nSEND-DM\nUser=acu\n\n2022";

    // Fork a child process to send data and another to receive it
    pid_t pid = fork();
    if (pid == 0)
    {
        // Client process
        // Init a socket with the server
        usleep(100 * 1000);
        int s_clientfd = setup_client_socket("127.0.0.1", "8082");

        int isOk = safe_send(s_clientfd, buf_in, sizeof(buf_in), 0) == 0;
        sleep(1);
        cr_assert(isOk, "safe_write failed");
        close(s_clientfd);
        exit(0);
    }
    else
    {
        // Server process
        // Init a socket with the client
        int s_listenfd = setup_server_socket("127.0.0.1", "8082");
        int s_clientfd = accept(s_listenfd, NULL, NULL);

        m = safe_recv(s_clientfd, 0, true);
        cr_expect_not_null(m);

        close(s_listenfd);
        close(s_clientfd);

        free_message(m);
        waitpid(pid, NULL, 0);
    }
}

Test(safe_io, safe_recv__slow_loris_hack)
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
        int s_clientfd = setup_client_socket("127.0.0.1", "8083");

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
        int s_listenfd = setup_server_socket("127.0.0.1", "8083");
        int s_clientfd = accept(s_listenfd, NULL, NULL);

        m = safe_recv(s_clientfd, 0, true);
        cr_expect_null(m);

        close(s_listenfd);
        close(s_clientfd);

        kill(pid, SIGKILL);
    }
}
