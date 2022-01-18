#include "message.h"
int main(void)
{
    struct message *r = parse_message("9223372036854775807\n9223372036854775807"
                                      "\nSEND-DM\nUser=acu\nFrom=ING1\n\n");
    free_message(r);
    return 0;
}
