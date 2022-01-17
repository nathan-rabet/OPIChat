#include "request_parser.h"

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type_check.h"
#include "xalloc.h"

/**
 * @brief Get the next key value object
 *
 * @warning This function modifies the input string
 *
 * @param params_start The start of the parameters
 * @param key The key of the object, NULL if no key
 * @param value The value of the object, NULL if no value
 * @return offset to the next object, 0 if no more object
 */
int get_next_key_value(char *params_start, char **key, char **value)
{
    *key = NULL;
    *value = NULL;

    regex_t regex;
    regmatch_t match[3]; // [0] = whole match, [1] = key, [2] = value,
                         // [3] = end of line
    int ret = regcomp(&regex, "([^=]+)=([^\n]+)(\n\n|\n)", REG_EXTENDED);
    if (ret != 0)
    {
        fprintf(stderr, "Error compiling regex\n");
        exit(1);
    }

    ret = regexec(&regex, params_start, 3, match, 0);
    regfree(&regex);
    if (ret != 0 || match[1].rm_so != 0)
        return 0;

    *key = params_start;
    params_start[match[1].rm_eo] = '\0'; // replace '=' with '\0'

    *value = params_start + match[2].rm_so;
    params_start[match[2].rm_eo] = '\0'; // replace '\n\n|\n' with '\0'

    return match[0].rm_eo;
}

struct request *parse_request(const char *request_string)
{
    if (!request_string)
        return NULL;

    struct request *r = xcalloc(1, sizeof(struct request));
    char *req_str_dup = strdup(request_string);

#define FREETURN(returned)                                                     \
    free(req_str_dup);                                                         \
    free_partial_request(r);                                                   \
    return (returned)

    char *line;
    char *saveptr;
    line = strtok_r(req_str_dup, "\n", &saveptr);
    if (!check_is_number(line))
    {
        FREETURN(NULL); // invalid request
    }
    r->payload_size = atoll(line);

    line = strtok_r(NULL, "\n", &saveptr);
    if (!check_is_number(line) || (r->status_code = atoll(line)) > 3)
    {
        FREETURN(NULL); // invalid request
    }

    line = strtok_r(NULL, "\n", &saveptr);
    if (!line)
    {
        FREETURN(NULL); // invalid request
    }
    r->command = strdup(line);

    // Determine the number of parameters
    char *key;
    char *value;

    // Get all key value pairs using get_next_key_value
    int offset;
    while ((offset = get_next_key_value(saveptr, &key, &value)) != 0)
    {
        r->command_parameters =
            xrealloc(r->command_parameters, (r->nb_parameters + 1),
                     sizeof(struct command_parameter));
        r->command_parameters[r->nb_parameters].key = strdup(key);
        r->command_parameters[r->nb_parameters].value = strdup(value);
        r->nb_parameters++;
        saveptr += offset;
    }

    line = strtok_r(NULL, "\n", &saveptr);

    if (line)
    {
        if ((strlen(line) != r->payload_size) /*No size lying is allowed :)*/
            || *saveptr != '\0' /*Last LF must be deleted*/)
        {
            FREETURN(NULL); // invalid request
        }

        r->payload = strdup(line);
    }

    free(req_str_dup);
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
