#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

extern struct client *clients;

int name_is_valid(char *s)
{
    if (!s)
        return 0;

    int i = 0;
    int isValid = 0;
    while (s[i] != '\0')
    {
        if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')
            || (s[i] >= '0' && s[i] <= '9'))
            isValid = 1;
        if (isValid == 0)
            return 0;
        isValid = 0;
        i++;
    }
    return 1;
}

int username_not_duplicate(char *name, struct client *clients,
                           struct client *me)
{
    if (name == NULL)
        return 1;

    struct client *current = clients;
    while(current != NULL)
    {
        if (current->client_socket != me->client_socket
            && current->username != NULL
            && strcmp(current->username, name) == 0)
            return 0;
        current = current->next;
    }
    return 1;
}

struct send_pool *handle_login(struct message *msg, struct client *client)
{
    struct message *response = NULL;

    if (name_is_valid(msg->payload) == 1)
    {
        if (username_not_duplicate(msg->payload, clients, client))
        {
            
            response = init_message(RESPONSE_MESSAGE_CODE);
            response->payload_size = strlen("Logged in");
            response->command = strdup("LOGIN");
            response->payload = strdup("Logged in");

            find_client(clients, client->client_socket)->username =
                strdup(msg->payload);
        }
        else
        {
            response = init_message(ERROR_MESSAGE_CODE);
            response->payload_size = strlen("Duplicate username");
            response->command = strdup("LOGIN");
            response->payload = strdup("Duplicate username");
        }
    }
    else
    {
        response = init_message(ERROR_MESSAGE_CODE);
        response->payload_size = strlen("Bad username");
        response->command = strdup("LOGIN");
        response->payload = strdup("Bad username");
    }

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
