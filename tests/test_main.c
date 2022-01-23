#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "message.h"
#include "read_from_stdin.h"
#include "client_read.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SERVER_IP SERVER_PORT\n", argv[0]);
        exit(1);
    }

    int socket = prepare_socket(argv[1], argv[2]);
    read_from_stdin(socket);
}

