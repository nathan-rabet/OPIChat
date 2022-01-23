#include <stddef.h>
#include <string.h>

#include "client.h"
#include "commands.h"
#include "message.h"
#include "xalloc.h"

static int length_of_names(struct client *client)
{
    struct client *current = client->next;
    int i = 0;
    while(current->username != NULL && strcmp(current->username, "") != 0)
    {
        i += strlen(current->username) + 1;
        current = current->next;
    }
    if(client->username != NULL && strcmp(client->username, "") != 0)
        i += strlen(client->username) + 1;
    return i + 1;
}

char *list_of_users(struct client *client)
{
    char *ret = xmalloc(length_of_names(client) + 1, sizeof(char));
    if(!ret)
        return NULL;
    struct client *current = client->next;

    while(current->username != NULL && strcmp(current->username, "") != 0)
        strcat(ret, current->username);
    if(client->username != NULL && strcmp(client->username, "") != 0)
        strcat(ret, current->username);
    return ret;
}
struct message *handle_list_users(struct message *msg, struct client *client)
{
    (void) msg;
    struct message *response = init_message(RESPONSE_MESSAGE_CODE);
    response->command = xmalloc(11, sizeof(char));
    strcpy(response->command, "LIST-USERS");
    response->payload = list_of_users(client);
    if(response->payload == NULL)
    {
        response->payload = xmalloc(1, sizeof(char));
        response->payload[0] = '\0';
    }
    response->payload_size = strlen(response->payload) + 1;
    return response;
}
