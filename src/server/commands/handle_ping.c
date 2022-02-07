#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

struct send_pool *handle_ping(struct message *msg, struct client *client)
{
    (void) client;
    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->payload_size = msg->payload_size;
    response->command = xmalloc(5, sizeof(char));
    strcpy(response->command, "PING");
    response->payload = xmalloc(5, sizeof(char));
    strcpy(response->payload, "PONG");

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
