#include <criterion/criterion.h>

#include "message.h"

Test(requests_parsing, subject_client_request)
{
    char req[] = "4\n0\nSEND-DM\nUser=acu\n\n2022";

    struct message *r = parse_message(req);

    cr_assert_eq(r->payload_size, 4, "r->payload_size = %d", r->payload_size);
    cr_assert_eq(r->status_code, 0, "r->status_code = %d", r->status_code);
    cr_assert_str_eq(r->command, "SEND-DM", "r->command = %s", r->command);
    cr_assert_eq(r->nb_parameters, 1, "r->nb_parameters = %d",
                 r->nb_parameters);
    cr_assert_str_eq(r->command_parameters[0].key, "User",
                     "r->command_parameters[0].key = %s",
                     r->command_parameters[0].key);
    cr_assert_str_eq(r->command_parameters[0].value, "acu",
                     "r->command_parameters[0].value = %s",
                     r->command_parameters[0].value);
    cr_assert_str_eq(r->payload, "2022", "r->payload = %s", r->payload);

    free_message(r);
}

Test(requests_parsing, subject_server_response)
{
    char req[] = "0\n1\nSEND-DM\nUser=acu\n\n";

    struct message *r = parse_message(req);

    cr_assert_eq(r->payload_size, 0, "r->payload_size = %d", r->payload_size);
    cr_assert_eq(r->status_code, 1, "r->status_code = %d", r->status_code);
    cr_assert_str_eq(r->command, "SEND-DM", "r->command = %s", r->command);
    cr_assert_eq(r->nb_parameters, 1, "r->nb_parameters = %d",
                 r->nb_parameters);
    cr_assert_str_eq(r->command_parameters[0].key, "User",
                     "r->command_parameters[0].key = %s",
                     r->command_parameters[0].key);
    cr_assert_str_eq(r->command_parameters[0].value, "acu",
                     "r->command_parameters[0].value = %s",
                     r->command_parameters[0].value);
    cr_assert_eq(r->payload, NULL);

    free_message(r);
}

Test(requests_parsing, subject_server_notification)
{
    char req[] = "4\n2\nSEND-DM\nUser=acu\nFrom=ING1\n\n2022";

    struct message *r = parse_message(req);

    cr_assert_eq(r->payload_size, 4, "r->payload_size = %d", r->payload_size);
    cr_assert_eq(r->status_code, 2, "r->status_code = %d", r->status_code);
    cr_assert_str_eq(r->command, "SEND-DM", "r->command = %s", r->command);
    cr_assert_eq(r->nb_parameters, 2, "r->nb_parameters = %d",
                 r->nb_parameters);
    cr_assert_str_eq(r->command_parameters[0].key, "User",
                     "r->command_parameters[0].key = %s",
                     r->command_parameters[0].key);
    cr_assert_str_eq(r->command_parameters[0].value, "acu",
                     "r->command_parameters[0].value = %s",
                     r->command_parameters[0].value);
    cr_assert_str_eq(r->command_parameters[1].key, "From",
                     "r->command_parameters[1].key = %s",
                     r->command_parameters[1].key);
    cr_assert_str_eq(r->command_parameters[1].value, "ING1",
                     "r->command_parameters[1].value = %s",
                     r->command_parameters[1].value);
    cr_assert_str_eq(r->payload, "2022");

    free_message(r);
}

Test(requests_parsing, key_value_is_payload)
{
    char req[] = "9\n2\nSEND-DM\nUser=acu\n\nFrom=ING1";

    struct message *r = parse_message(req);

    cr_assert_eq(r->payload_size, 9, "r->payload_size = %d", r->payload_size);
    cr_assert_eq(r->status_code, 2, "r->status_code = %d", r->status_code);
    cr_assert_str_eq(r->command, "SEND-DM", "r->command = %s", r->command);
    cr_assert_eq(r->nb_parameters, 1, "r->nb_parameters = %d",
                 r->nb_parameters);
    cr_assert_str_eq(r->command_parameters[0].key, "User",
                     "r->command_parameters[0].key = %s",
                     r->command_parameters[0].key);
    cr_assert_str_eq(r->command_parameters[0].value, "acu",
                     "r->command_parameters[0].value = %s",
                     r->command_parameters[0].value);
    cr_assert_str_eq(r->payload, "From=ING1");

    free_message(r);
}

Test(requests_parsing, no_command_parameter)
{
    char req[] = "9\n2\nSEND-DM\nFrom=ING1";

    struct message *r = parse_message(req);

    cr_assert_eq(r->payload_size, 9, "r->payload_size = %d", r->payload_size);
    cr_assert_eq(r->status_code, 2, "r->status_code = %d", r->status_code);
    cr_assert_str_eq(r->command, "SEND-DM", "r->command = %s", r->command);
    cr_assert_eq(r->nb_parameters, 0, "r->nb_parameters = %d",
                 r->nb_parameters);
    cr_assert_eq(r->command_parameters, NULL);
    cr_assert_str_eq(r->payload, "From=ING1");

    free_message(r);
}

Test(requests_parsing, multiple_command_parameters)
{
    char req[] = "4\n2\nSEND-DM\na=aa\nb=bb\nc=cc\nd=dd\n\n2022";

    struct message *r = parse_message(req);

    cr_assert_eq(r->payload_size, 4, "r->payload_size = %d", r->payload_size);
    cr_assert_eq(r->status_code, 2, "r->status_code = %d", r->status_code);
    cr_assert_str_eq(r->command, "SEND-DM", "r->command = %s", r->command);
    cr_assert_eq(r->nb_parameters, 4, "r->nb_parameters = %d",
                 r->nb_parameters);
    cr_assert_str_eq(r->command_parameters[0].key, "a",
                     "r->command_parameters[0].key = %s",
                     r->command_parameters[0].key);
    cr_assert_str_eq(r->command_parameters[0].value, "aa",
                     "r->command_parameters[0].value = %s",
                     r->command_parameters[0].value);
    cr_assert_str_eq(r->command_parameters[1].key, "b",
                     "r->command_parameters[1].key = %s",
                     r->command_parameters[1].key);
    cr_assert_str_eq(r->command_parameters[1].value, "bb",
                     "r->command_parameters[1].value = %s",
                     r->command_parameters[1].value);
    cr_assert_str_eq(r->command_parameters[2].key, "c",
                     "r->command_parameters[2].key = %s",
                     r->command_parameters[2].key);
    cr_assert_str_eq(r->command_parameters[2].value, "cc",
                     "r->command_parameters[2].value = %s",
                     r->command_parameters[2].value);
    cr_assert_str_eq(r->command_parameters[3].key, "d",
                     "r->command_parameters[3].key = %s",
                     r->command_parameters[3].key);
    cr_assert_str_eq(r->command_parameters[3].value, "dd",
                     "r->command_parameters[3].value = %s",
                     r->command_parameters[3].value);

    cr_assert_str_eq(r->payload, "2022");

    free_message(r);
}

Test(requests_parsing, empty_request)
{
    char req[] = "";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(requests_parsing, wrong_payload_size)
{
    char req[] = "69\n2\nSEND-DM\nUser=acu\nFrom=ING1\n\n2022";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(requests_parsing, byte_instead_of_char)
{
    char req[] = "\x04\n\x02\nSEND-DM\nUser=acu\nFrom=ING1\n\n2\022";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(requests_parsing, bad_input)
{
    char req[] = "quatre\ndeux\nexecvp\nUser=/bin/sh\nnotkvpair\n\n";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(requests_parsing, no_line_feed)
{
    char req[] = "42SEND-DMUser=acuFrom=ING12022";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(requests_parsing, random_request)
{
    char req[] = "iaohfifhaofihaofh ofh ozehf zoh fozei fhzo i";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(requests_parsing, huge_llong)
{
    char req[] = "9223372036854775807\n9223372036854775807\nSEND-DM\nUser="
                 "acu\nFrom=ING1\n\n";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(requests_parsing, huge_llong_2)
{
    char req[] = "4\n9223372036854775807\nSEND-DM\nUser="
                 "acu\nFrom=ING1\n\ntest";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(requests_parsing, huge_llong_3)
{
    char req[] = "4n9223372036854775807\n0\nSEND-DM\nUser="
                 "acu\nFrom=ING1\n\ntest";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(command_parameter, only_one_LF)
{
    char req[] = "7\n2\nSEND-DM\nUser=acu\nYolerap";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(command_parameter, end_LFLF)
{
    char req[] = "7\n2\nSEND-DM\nUser=acu\nYolerap\n\n";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(command_parameter, only_one_LF_with_kv_as_payload)
{
    char req[] = "4\n2\nSEND-DM\nUser=acu\nFrom=ING1";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(command_parameter, invalid_key)
{
    char req[] = "4\n2\nSEND-DM\n=acu\nFrom=ING1\n\n2022";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}

Test(command_parameter, invalid_value)
{
    char req[] = "4\n2\nSEND-DM\nUser=\nFrom=ING1\n\n2022";

    struct message *r = parse_message(req);

    cr_assert_eq(r, NULL);
}
