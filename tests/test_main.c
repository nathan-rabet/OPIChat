#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "client_read.h"
#include "init_socket.h"
#include "message.h"
#include "client.h"


int main(void)
{
    struct client *client1 = NULL;
    struct client *client2 = NULL;
    struct sockaddr addr = {
        .sa_family = AF_INET,
    };

    client1 = add_client(client1, 1, addr, sizeof(addr));
    client2 = add_client(client1, 2, addr, sizeof(addr));

    client2 = remove_client(client2, 1);
    client2 = remove_client(client2, 2);

}