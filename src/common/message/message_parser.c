#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "type_check.h"
#include "xalloc.h"
struct message *parse_message(const char *request_string)
{
    if (!request_string)
        return NULL;

    struct message *r = xcalloc(1, sizeof(struct message));
    char *req_str_dup = strdup(request_string);

#define FREETURN(returned)                                                     \
    free(req_str_dup);                                                         \
    free_partial_message(r);                                                   \
    return (returned)

    char *line;
    char *saveptr;
    line = strtok_r(req_str_dup, "\n", &saveptr);
    if (*saveptr == '\0' || !check_is_number(line))
    {
        FREETURN(NULL); // invalid message
    }
    r->payload_size = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    if (*saveptr == '\0' || (!check_is_number(line) || atoll(line) > 3))
    {
        FREETURN(NULL); // invalid message
    }

    r->status_code = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    if (!line || *saveptr == '\0')
    {
        FREETURN(NULL); // invalid message
    }
    r->command = strdup(line);

    // Determine the number of parameters
    char *key = NULL;
    char *value = NULL;

    // Get all key value pairs using get_message_next_parameter_kv
    int offset;
    while ((offset = get_message_next_parameter_kv(saveptr, &key, &value)) != 0)
    {
        // Check if duplicate keys
        for (uint64_t i = 0; i < r->nb_parameters; i++)
            if (strcmp(r->command_parameters[i].key, key) == 0)
            {
                FREETURN(NULL); // invalid message
            }

        r->command_parameters =
            xrealloc(r->command_parameters, (r->nb_parameters + 1),
                     sizeof(struct command_parameter));
        r->command_parameters[r->nb_parameters].key = strdup(key);
        r->command_parameters[r->nb_parameters].value = strdup(value);
        r->nb_parameters++;
        saveptr += offset;
    }

    if (*saveptr++ != '\n')
    {
        FREETURN(NULL); // invalid message
    }

    if (*saveptr)
    {
        if ((strlen(saveptr) != r->payload_size))
        {
            FREETURN(NULL); // invalid message
        }

        r->payload = strndup(saveptr, r->payload_size);
    }

    else if (r->payload_size != 0)
    {
        FREETURN(NULL); // invalid message
    }

    else
    {
        r->payload = strdup("");
    }

    free(req_str_dup);
    return r;
}
