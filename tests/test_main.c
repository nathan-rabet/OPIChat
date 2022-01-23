#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

#include "message.h"
#include "read_from_stdin.h"
#include "client_read.h"

/*int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SERVER_IP SERVER_PORT\n", argv[0]);
        exit(1);
    }

    int socket = setup_client_socket(argv[1], argv[2]);
    pthread_t *send_message = NULL;
    pthread_t *receive_from_server = NULL;
    int ret_send =  pthread_create (send_message, NULL, &read_from_stdin_thread, &socket);
    int ret_recv = pthread_create (receive_from_server, NULL, &read_thread, &socket);
    pthread_join(ret_send, NULL);
    printf("ret_recv = %d\n", ret_recv);
    printf("ret_send = %d\n", ret_send);
    return 0;
}*/

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SERVER_IP SERVER_PORT\n", argv[0]);
        exit(1);
    }

    int socket = setup_client_socket(argv[1], argv[2]);
    read_from_stdin(socket);
}
