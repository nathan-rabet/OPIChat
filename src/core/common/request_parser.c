#include "request_parser.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "type_check.h"
#include "xalloc.h"

struct request *parse_request(const char *request_string)
{
    if (!request_string)
        return NULL;

    struct request *r = xcalloc(1, sizeof(struct request));

    char *req_str_dup = strdup(request_string);
    char *line;
    char *saveptr;
    line = strtok_r(req_str_dup, "\n", &saveptr);
    if (!check_is_number(line))
    {
        free_partial_request(r);
        return NULL;
    }
    r->payload_size = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    if (!check_is_number(line))
    {
        free_partial_request(r);
        return NULL;
    }
    r->status_code = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    if (!line)
    {
        free_partial_request(r);
        return NULL;
    }
    r->command = strdup(line);

    // Determine the number of parameters
    char *token;
    line = strtok_r(NULL, "\n\n", &saveptr);
    char *save_payload_ptr = saveptr;

    while (*saveptr != '\0')
    {
        // Check overflow
        size_t command_parameter_size = 0;
        if (__builtin_umull_overflow(sizeof(struct command_parameter),
                                     r->nb_parameters + 1,
                                     &command_parameter_size))
        {
            free_partial_request(r);
            return NULL;
        }

        r->command_parameters =
            xrealloc(r->command_parameters, 1, command_parameter_size);

        token = strtok_r(line, "=", &saveptr);
        if (!token)
        {
            free_partial_request(r);
            return NULL;
        }
        r->command_parameters[r->nb_parameters].key = strdup(token);

        token = strtok_r(NULL, "\n", &saveptr);
        if (!token)
        {
            free_partial_request(r);
            return NULL;
        }
        r->command_parameters[r->nb_parameters].value = strdup(token);
        r->nb_parameters++;
    }

    line = strtok_r(NULL, "\n", &save_payload_ptr);

    if (line)
    {
        if ((strlen(line) != r->payload_size) /*No size lying is allowed :)*/
            || *save_payload_ptr != '\0' /*Last LF must be deleted*/)
        {
            free_partial_request(r);
            return NULL;
        }
        r->payload = strdup(line);
    }

    return r;
}

void free_request(struct request *request)
{
    free(request->command);
    for (uint64_t i = 0; i < request->nb_parameters; i++)
    {
        free(request->command_parameters[i].key);
        free(request->command_parameters[i].value);
    }
    free(request->command_parameters);
    free(request->payload);
    free(request);
}

void free_partial_request(struct request *request)
{
    if (request)
    {
        if (request->command)
            free(request->command);
        for (uint64_t i = 0; i < request->nb_parameters; i++)
        {
            if (request->command_parameters[i].key)
                free(request->command_parameters[i].key);
            if (request->command_parameters[i].value)
                free(request->command_parameters[i].value);
        }
        if (request->command_parameters)
            free(request->command_parameters);
        if (request->payload)
            free(request->payload);
        free(request);
    }
}
