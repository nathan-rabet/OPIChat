#include "message.h"
#include "xalloc.h"

struct message *init_message(uint8_t status_code)
{
    struct message *message = xmalloc(1, sizeof(struct message));
    if (!message)
        return NULL;
    message->command = NULL;
    message->nb_parameters = 0;
    message->payload = NULL;
    message->payload_size = 0;
    message->status_code = status_code;
    message->command_parameters = NULL;
    return message;
}