#include <criterion/criterion.h>

#include "message.h"


Test(message_serializing, subject_client_response)
{
    char req[] = "4\n0\nSEND-DM\nUser=acu\n\n2022";

    struct message *r = parse_message(req);
    char *serialized = compose_message(r);
    cr_assert_str_eq(serialized, "4\n0\nSEND-DM\nUser=acu\n\n2022");

    free_message(r);
    free(serialized);
}
Test(message_serializing, subject_server_notification)
{
    char req[] = "4\n2\nSEND-DM\nUser=acu\nFrom=ING1\n\n2022";

    struct message *r = parse_message(req);
    char *serialized = compose_message(r);
    cr_assert_str_eq(serialized, "4\n2\nSEND-DM\nUser=acu\nFrom=ING1\n\n2022");
    free_message(r);
    free(serialized);
}

Test(message_serializing, key_value_is_payload)
{
    char req[] = "9\n2\nSEND-DM\nUser=acu\n\nFrom=ING1";

    struct message *r = parse_message(req);
    char *serialized = compose_message(r);
    cr_assert_str_eq(serialized, req);
    free_message(r);
    free(serialized);

}

Test(message_serializing, no_command_parameter)
{
    char req[] = "9\n2\nSEND-DM\nFrom=ING1";

    struct message *r = parse_message(req);
    char *serialized = compose_message(r);
    cr_assert_str_eq(serialized, req);
    free_message(r);
    free(serialized);
}

Test(message_serializing, multiple_command_parameters)
{
    char req[] = "4\n2\nSEND-DM\na=aa\nb=bb\nc=cc\nd=dd\n\n2022";

    struct message *r = parse_message(req);
    char *serialized = compose_message(r);
    cr_assert_str_eq(serialized, req);
    free_message(r);
    free(serialized);
}


Test(message_serializing, empty_message)
{
    char *serialized = compose_message(NULL);
    cr_assert_eq(serialized, NULL);
}