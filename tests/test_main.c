#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "client.h"
#include "client_read.h"
#include "commands.h"
#include "init_socket.h"
#include "message.h"
#include "read_from_stdin.h"
#include "safe_io.h"

int main(void)
{
    struct message *message = init_message(REQUEST_MESSAGE_CODE);
    message->command = "PING";
    message->payload_size = 4;
    message->payload = xmalloc(5, sizeof(char));
    strcpy(message->payload, "PONG");

    struct send_pool *sp = handle_ping(message, NULL);

    (void)sp;
}
