#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "message.h"

int main(void)
{
    char req[] = "0\n1\nSEND-DM\nUser=acu\n\n";

    struct message *r = parse_message(req);

    printf("%s\n", r->payload);
}
