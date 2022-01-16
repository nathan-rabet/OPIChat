#include "request_parser.h"

#include <stdlib.h>
#include <string.h>

#include "xalloc.h"

struct request *parse_request(char *request_string)
{
    struct request *r = xmalloc(1, sizeof(struct request));

    char *line;
    char *saveptr;
    line = strtok_r(request_string, "\n", &saveptr);
    r->payload_size = atoi(line);

    line = strtok_r(NULL, "\n", &saveptr);
    r->status_code = atoi(line);

    line = strtok_r(NULL, "\n", &saveptr);
    r->command = strdup(line);

    line = strtok_r(NULL, "\n", &saveptr);
    r->nb_parameters = atoi(line);

    r->command_parameters =
        xmalloc(r->nb_parameters, sizeof(struct command_parameter));

    for (uint64_t i; i < r->nb_parameters; i++)
    {
        char *token;

        line = strtok_r(NULL, "\n", &saveptr);
        token = strtok_r(line, "=", &saveptr);
        r->command_parameters[i].key = strdup(token);
        token = strtok_r(NULL, "=", &saveptr);
        r->command_parameters[i].value = strdup(token);
    }

    line = strtok_r(NULL, "\n", &saveptr);
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
