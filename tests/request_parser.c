#include "request_parser.h"

#include <criterion/criterion.h>

Test(requests_parsing, subject_client_request)
{
    char req[] = "4\n0\nSEND-DM\nUser=acu\n\n2022";

    struct request *r = parse_request(req);

    cr_assert_eq(r->payload_size, 4);
    cr_assert_eq(r->status_code, 0);
    cr_assert_str_eq(r->command, "SEND-DM");
    cr_assert_eq(r->nb_parameters, 1);
    cr_assert_str_eq(r->command_parameters[0].key, "User");
    cr_assert_str_eq(r->command_parameters[0].value, "acu");
    cr_assert_str_eq(r->payload, "2022");

    free_request(r);
}

Test(requests_parsing, subject_server_response)
{
    char req[] = "0\n1\nSEND-DM\nUser=acu\n\n";

    struct request *r = parse_request(req);

    cr_assert_eq(r->payload_size, 0);
    cr_assert_eq(r->status_code, 1);
    cr_assert_str_eq(r->command, "SEND-DM");
    cr_assert_eq(r->nb_parameters, 1);
    cr_assert_str_eq(r->command_parameters[0].key, "User");
    cr_assert_str_eq(r->command_parameters[0].value, "acu");
    cr_assert_eq(r->payload, NULL);

    free_request(r);
}

Test(requests_parsing, subject_server_notification)
{
    char req[] = "4\n2\nSEND-DM\nUser=acu\nFrom=ING1\n\n2022";

    struct request *r = parse_request(req);

    cr_assert_eq(r->payload_size, 4);
    cr_assert_eq(r->status_code, 2);
    cr_assert_str_eq(r->command, "SEND-DM");
    cr_assert_eq(r->nb_parameters, 2);
    cr_assert_str_eq(r->command_parameters[0].key, "User");
    cr_assert_str_eq(r->command_parameters[0].value, "acu");
    cr_assert_str_eq(r->command_parameters[1].key, "From");
    cr_assert_str_eq(r->command_parameters[1].value, "ING1");
    cr_assert_str_eq(r->payload, "2022");

    free_request(r);
}
