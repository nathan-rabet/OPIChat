#include "request_parser.h"
int main(void)
{
    struct request *r = parse_request("4\n0\nSEND-DM\nUser=acu\n\n2022");
    free_request(r);
    return 0;
}
