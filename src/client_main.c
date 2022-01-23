#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#include "init_socket.h"
#include "logger.h"
#include "read_from_server.h"
#include "read_from_stdin.h"

extern int server_socket;
extern char *ip;
extern char *port;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    logger_init("client.log");

    while ((server_socket = setup_client_socket(argv[1], argv[2])) == -1)
    {
        write_error("Impossible to connect to server %s:%s, retrying... : %s",
                    argv[1], argv[2], strerror(errno));
        sleep(1);
    }

    ip = argv[1];
    port = argv[2];

    pthread_t read_from_server_thread_instance;
    pthread_t read_from_stdin_thread_instance;

    pthread_create(&read_from_server_thread_instance, NULL,
                   read_from_server_thread, NULL);
    pthread_create(&read_from_stdin_thread_instance, NULL,
                   read_from_stdin_thread, NULL);

    pthread_join(read_from_server_thread_instance, NULL);
    pthread_join(read_from_stdin_thread_instance, NULL);

    return 0;
}
