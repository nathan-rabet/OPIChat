#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "my_itoa.c"
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
            i += strlen(message->command_parameters[j].key) + 1;
            i += 64; // Must be changed to optimize memory and for long messages
        }
    }
    if (message->payload_size != 0 && message->payload != NULL)
        i += strlen(message->payload) + 1;
    return i + 1;
}


char *compose_message(struct message *message)
{
    if (message == NULL)
        return NULL;

    char *ret = xmalloc(1, message_size(message)); // Compute expected length of message
    char *tmp; // Values that we will concatenate to our main mesage
    ret[0] = '\0';
    //strcat(ret, payload_size)
    if (message->payload_size != 0) // if payload is not empty
    {
        tmp = xmalloc(number_of_decimal(message->payload_size) + 1, sizeof(char));
        strcat(ret, my_itoa(message->payload_size, tmp));
        free(tmp);
        strcat(ret, "\n");
    }
    //strcat(ret, status_code)
    tmp = xmalloc(number_of_decimal(message->status_code) + 1, sizeof(char));
    strcat(ret, my_itoa(message->status_code, tmp));
    free(tmp);
    strcat(ret, "\n");

    //strcat(ret, command) and command parameters
    if ( message->command != NULL && strcmp(message->command, "SEND-DM") == 0)
    {
        if(strcmp(message->command, "") != 0)
        {
            strcat(ret, message->command);
            strcat(ret, "\n");
        }
        unsigned i;
        for (i = 0; i < message->nb_parameters; i++)
        {
            strcat(ret, message->command_parameters[i].key);
            strcat(ret, "=");
            strcat(ret, message->command_parameters[i].value);
            strcat(ret, "\n");
        }
        if (i != 0)
            strcat(ret, "\n");

    }
    strcat(ret, message->payload);
    ret = xrealloc(ret, strlen(ret) + 1,  sizeof(char)); // In case too much memory was allocated
    return ret;
}