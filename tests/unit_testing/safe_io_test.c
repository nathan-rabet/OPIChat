#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "client_read.h"
#include "init_socket.h"
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

Test(safe_io, safe_send)
{
    char buf_in[] = { 'H', 'e', 'l', 'l', 'o', ' ',
                      'W', 'o', 'r', 'l', 'd', '\0' };

    int sockfd = setup_socket("127.0.0.1", "8080");
    cr_assert(sockfd != -1, "safe_send failed");

    // Send test
    int isOk = safe_send(sockfd, buf_in, sizeof(buf_in), 0) == 0;
    cr_assert(isOk, "safe_send failed");

    close(sockfd);
}

Test(safe_io, safe_recv)
{
    char buf_out[1024] = { 0 };
    char buf_in[] = { 'H', 'e', 'l', 'l', 'o', ' ',
                      'W', 'o', 'r', 'l', 'd', '\0' };

    // Fork a child process to send data and another to receive it
    pid_t pid = fork();
    if (pid == 0)
    {
        // Client process
        // Init a socket with the server
        int clientfd = prepare_socket("127.0.0.1", "8080");
        cr_assert(clientfd != -1, "safe_recv failed");

        sleep(1);
        safe_write(clientfd, buf_in, sizeof(buf_in));
        close(clientfd);
        exit(0);
    }
    else
    {
        // Server process
        // Init a socket with the client
        int sockfd = setup_socket("127.0.0.1", "8080");
        cr_assert(sockfd != -1, "safe_recv failed");

        int nb_read = safe_recv(sockfd, (void **)&buf_out, 0);
        cr_expect_eq(nb_read, sizeof(buf_in), "Actual number of bytes read: %d",
                     nb_read);
        cr_expect_str_eq(buf_out, buf_in);

        close(sockfd);
        waitpid(pid, NULL, 0);
    }
}
