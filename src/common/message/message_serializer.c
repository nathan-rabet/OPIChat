#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "my_itoa.h"
#include "xalloc.h"

static int number_of_decimal(int i)
{
    int j = 1;
    while ((i = i / 10) != 0)
        j++;
    return j;
}

static int message_size(struct message *message)
{
    int i = 0;
    i += number_of_decimal(message->payload_size) + 1;
    i += 1;
    if (message->command != NULL)
        i += strlen(message->command) + 1;
    if (message->nb_parameters != 0)
        i += number_of_decimal(message->nb_parameters) + 1;
    if (message->command_parameters != NULL)
    {
        for (unsigned j = 0; j < message->nb_parameters; j++)
        {
            i += strlen(message->command_parameters[j].key) + 2;
            i += strlen((char *)message->command_parameters[j].value) + 2;
        }
    }
    if (message->payload_size != 0 && message->payload != NULL)
        i += strlen(message->payload) + 1;
    i += 1;  // If we do not have a payload

    return i + 3;
}

char *compose_message(struct message *message)
{
    if (message == NULL)
        return NULL;

    char *ret =
        xmalloc(1, message_size(message)); // Compute expected length of message
    char *tmp; // Values that we will concatenate to our main mesage
    ret[0] = '\0';

    // strcat(ret, payload_size)
    tmp = xmalloc(number_of_decimal(message->payload_size) + 1, sizeof(char));
    strcat(ret, my_itoa(message->payload_size, tmp));
    free(tmp);
    strcat(ret, "\n");

    // strcat(ret, status_code)
    tmp = xmalloc(number_of_decimal(message->status_code) + 1, sizeof(char));
    strcat(ret, my_itoa(message->status_code, tmp));
    free(tmp);
    strcat(ret, "\n");

    if (message->command != NULL && strcmp(message->command, "") != 0)
    {
        strcat(ret, message->command);
        strcat(ret, "\n");
    }

    // strcat(ret, command) and command parameters
    if (message->command != NULL && strcmp(message->command, "SEND-DM") == 0)
    {
        unsigned i;
        for (i = 0; i < message->nb_parameters; i++)
        {
            strcat(ret, message->command_parameters[i].key);
            strcat(ret, "=");
            strcat(ret, message->command_parameters[i].value);
            strcat(ret, "\n");
        }
    }

    strcat(ret, "\n");
    if (message->payload_size != 0)
    {
        ret = xrealloc(ret, strlen(ret) + strlen(message->payload) + 3,
                       sizeof(char));
        strncat(ret, message->payload, message->payload_size);
    }

    ret = xrealloc(ret, strlen(ret) + 1,
                   sizeof(char)); // In case too much memory was allocated
    return ret;
}
