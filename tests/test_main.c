#include "message.h"
int main(void)
{
    struct message *r = parse_message("7\n2\nSEND-DM\nUser=acu\nYolerap");
    free_partial_message(r);
    return 0;
}
