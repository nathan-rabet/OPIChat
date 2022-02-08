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
    char req[] = "4\n2\nSEND-DM\nUser=acu\nFrom=ING1\n\n2022";
    struct message *r = parse_message(req);

    (void)r;
}
