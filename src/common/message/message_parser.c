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
    if (!check_is_number(line))
    {
        FREETURN(NULL); // invalid message
    }
    r->payload_size = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    if (!check_is_number(line) || atoll(line) > 3)
    {
        FREETURN(NULL); // invalid message
    }

    r->status_code = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    if (!line)
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
        r->command_parameters =
            xrealloc(r->command_parameters, (r->nb_parameters + 1),
                     sizeof(struct command_parameter));
        r->command_parameters[r->nb_parameters].key = strdup(key);
        r->command_parameters[r->nb_parameters].value = strdup(value);
        r->nb_parameters++;
        saveptr += offset;
    }

    if (value && *(saveptr - 1) == '\0')
    {
        FREETURN(NULL); // invalid message
    }

    line = strtok_r(NULL, "\n", &saveptr);

    if (line)
    {
        if ((strlen(line) != r->payload_size) /*No size lying is allowed :)*/
            || *saveptr != '\0' /*Last LF must be deleted*/)
        {
            FREETURN(NULL); // invalid message
        }

        r->payload = strdup(line);
    }

    free(req_str_dup);
    return r;
}
