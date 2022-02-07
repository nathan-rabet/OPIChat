#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

extern struct client *clients;

int username_is_valid(char *s)
{
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

int username_not_duplicate(char *name, struct client *client)
{
    if (name == NULL)
        return 1;

    struct client *current = client;
    while(current != NULL)
    {
        if (current->username != NULL && strcmp(current->username, name) == 0)
            return 0;
        current = current->next;
    }
    return 1;
}

struct send_pool *handle_login(struct message *msg, struct client *client)
{
    struct message *response = NULL;

    if(username_is_valid(msg->payload) == 1)
    {
        if (username_not_duplicate(msg->payload, clients))
        {
            
            response = init_message(RESPONSE_MESSAGE_CODE);
            response->payload_size = strlen("Logged in") + 1;
            response->command = xmalloc(6, sizeof(char));
            strcpy(response->command, "LOGIN");
            response->payload = xmalloc(strlen("Logged in") + 1, sizeof(char));
            strcpy(response->payload, "Logged in");
            client->username =
                xrealloc(client->username, msg->payload_size + 1, sizeof(char));
            strcpy(client->username, msg->payload);
        }
        else
        {
            response = init_message(ERROR_MESSAGE_CODE);
            response->payload_size = strlen("Duplicate username") + 1;
            response->command = xmalloc(6, sizeof(char));
            strcpy(response->command, "LOGIN");
            response->payload = xmalloc(response->payload_size, sizeof(char));
            strcpy(response->payload, "Duplicate username");
        }
    }
    else
    {
        response = init_message(ERROR_MESSAGE_CODE);
        response->payload_size = strlen("Bad username");
        response->command = xmalloc(6, sizeof(char));
        strcpy(response->command, "LOGIN");
        response->payload = xmalloc(strlen("Bad username") + 1, sizeof(char));
        strcpy(response->payload, "Bad username");
    }

    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));

    sp->nb_msg = 1;
    sp->msg = xmalloc(1, sizeof(struct message *));
    sp->clients_sockets = xmalloc(1, sizeof(int));
    sp->msg[0] = response;
    sp->clients_sockets[0] = client->client_socket;

    return sp;
}
