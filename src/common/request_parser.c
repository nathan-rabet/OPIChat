#include "request_parser.h"

#include <stdlib.h>
#include <string.h>

#include "xalloc.h"

struct request *parse_request(char *request_string)
{
    struct request *r = xcalloc(1, sizeof(struct request));

    char *req_str_dup = strdup(request_string);

    char *line;
    char *saveptr;
    line = strtok_r(req_str_dup, "\n", &saveptr);
    r->payload_size = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    r->status_code = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    r->command = strdup(line);

    // Determine the number of parameters
    char *token;
    line = strtok_r(NULL, "\n\n", &saveptr);
    char *save_payload_ptr = saveptr;

    while (*saveptr != '\0')
    {
        r->command_parameters =
            xrealloc(r->command_parameters, ++(r->nb_parameters),
                     sizeof(struct command_parameter));

        token = strtok_r(line, "=", &saveptr);
        r->command_parameters[r->nb_parameters].key = strdup(token);
        token = strtok_r(NULL, "\n", &saveptr);
        r->command_parameters[r->nb_parameters].value = strdup(token);
    }

    line = strtok_r(NULL, "\n", &save_payload_ptr);
    r->payload = strdup(line);

    return r;
}

void free_request(struct request *request)
{
    free(request->command);
    for (uint64_t i; i < request->nb_parameters; i++)
    {
        free(request->command_parameters[i].key);
        free(request->command_parameters[i].value);
    }
    free(request->command_parameters);
    free(request->payload);
    free(request);
}
