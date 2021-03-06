#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#include "message.h"

int get_message_next_parameter_kv(char *params_start, char **key, char **value)
{
    regex_t regex;
    regmatch_t match[4]; // [0] = whole match, [1] = key, [2] = value,
                         // [3] = end of line
    int ret = regcomp(&regex, "^([^=]+)=([^\n]+)(\n)", REG_EXTENDED);
    if (ret != 0)
    {
        fprintf(stderr, "Error compiling regex\n");
        exit(1);
    }

    ret = regexec(&regex, params_start, 4, match, 0);
    regfree(&regex);
    if (ret != 0)
        return 0;

    *key = params_start;
    params_start[match[1].rm_eo] = '\0'; // replace '=' with '\0'

    *value = params_start + match[2].rm_so;
    params_start[match[2].rm_eo] = '\0'; // replace '\n\n|\n' with '\0'

    return match[0].rm_eo;
}
