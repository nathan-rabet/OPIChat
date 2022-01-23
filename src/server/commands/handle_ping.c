#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

struct message *handle_ping(struct message *msg, struct client *client)
{
    (void) client;
    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->payload_size = msg->payload_size;
    response->command = xmalloc(5, sizeof(char));
    strcpy(response->command, "PING");
    response->payload = xmalloc(5, sizeof(char));
    strcpy(response->payload, "PONG");
    return response;
}
