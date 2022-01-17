#include "request_parser.h"
int main(void)
{
    struct request *r =
        parse_request("4\n2\nSEND-DM\na=aa\nb=bb\nc=cc\nd=dd\n\n2022");
    free_request(r);
    return 0;
}
