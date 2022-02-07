#include <stdlib.h>

#include "message.h"

void free_message(struct message *message)
{
    free(message->command);
    for (uint64_t i = 0; i < message->nb_parameters; i++)
    {
        free(message->command_parameters[i].key);
        free(message->command_parameters[i].value);
    }
    free(message->command_parameters);
    free(message->payload);
    free(message);
}

void free_partial_message(struct message *message)
{
    if (message)
    {
        if (message->command)
            free(message->command);
        if (message->command_parameters)
            for (uint64_t i = 0; i < message->nb_parameters; i++)
            {
                if (message->command_parameters[i].key)
                    free(message->command_parameters[i].key);
                if (message->command_parameters[i].value)
                    free(message->command_parameters[i].value);
            }
        if (message->command_parameters)
            free(message->command_parameters);
        if (message->payload)
            free(message->payload);
        free(message);
    }
}
