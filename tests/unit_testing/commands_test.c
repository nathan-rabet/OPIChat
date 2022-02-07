#include <criterion/criterion.h>
#include <stdio.h>

#include "commands.h"
#include "xalloc.h"

Test(commands, ping_empty_payload)
{
    struct message *message = init_message(REQUEST_MESSAGE_CODE);
    message->command = "PING";
    message->payload_size = 4;
    message->payload = xmalloc(5, sizeof(char));
    strcpy(message->payload, "PONG");

    struct sockaddr addr = {
        .sa_family = AF_INET,
    };

    struct client *client = add_client(NULL, 1, addr, sizeof(addr));

    struct send_pool *sp = handle_ping(message, client);
    cr_assert_eq(sp->msg[0]->status_code, RESPONSE_MESSAGE_CODE);
    cr_assert_eq(sp->msg[0]->payload_size, 4);
    cr_assert_eq(sp->msg[0]->nb_parameters, 0);
    cr_assert_str_eq(sp->msg[0]->payload, "PONG");

    free_message(sp->msg[0]);
    free(sp->msg);
    free(sp);
    free(client);
}

Test(commands, ping_with_payload)
{
    struct message *message = init_message(REQUEST_MESSAGE_CODE);
    message->command = "PING";
    message->payload_size = strlen("skkkkkkkrt") + 1;
    message->payload = malloc(sizeof(char) * 5);
    strcpy(message->payload, "skkkkkkkrt");

    struct sockaddr addr = {
        .sa_family = AF_INET,
    };

    struct client *client = add_client(NULL, 1, addr, sizeof(addr));

    struct send_pool *sp = handle_ping(message, client);
    cr_assert_eq(sp->msg[0]->status_code, RESPONSE_MESSAGE_CODE);
    cr_assert_eq(sp->msg[0]->payload_size, 4);
    cr_assert_eq(sp->msg[0]->nb_parameters, 0);
    cr_assert_eq(strcmp(sp->msg[0]->payload, "PONG"), 0);

    free_message(sp->msg[0]);
    free(sp->msg);
    free(sp);
    free(client);
}

Test(commands, dup_true)
{
    struct client *client1 = xmalloc(1, sizeof(struct client));
    struct client *client2 = xmalloc(1, sizeof(struct client));
    client1->username = NULL;
    client2->username = NULL;
    client1->next = client2;
    client2->next = NULL;
    client1->username = xmalloc(strlen("bonjour") + 1, sizeof(char));
    client2->username = xmalloc(strlen("bonj") + 1, sizeof(char));
    strcpy(client1->username, "bonjour");
    strcpy(client2->username, "bonj");
    cr_assert_eq(username_not_duplicate("bonjour", client1, client2), 0);
    free(client1->username);
    free(client1);
    free(client2->username);
    free(client2);

}

Test(commands, dup_false)
{
    struct client *client1 = xmalloc(1, sizeof(struct client));
    struct client *client2 = xmalloc(1, sizeof(struct client));
    client1->username = NULL;
    client2->username = NULL;
    client1->next = client2;
    client2->next = NULL;
    client1->username = xmalloc(strlen("bonjour") + 1, sizeof(char));
    client2->username = xmalloc(strlen("b") + 1, sizeof(char));
    strcpy(client1->username, "bonjour");
    strcpy(client2->username, "bonj");
    cr_assert_eq(username_not_duplicate("daeedja", client1, client2), 1);
    free(client1->username);
    free(client1);
    free(client2->username);
    free(client2);
}
/*


Test(commands, login_valid_username)
{
    struct client *client1 = xmalloc(1, sizeof(struct client));
    struct client *client2 = xmalloc(1, sizeof(struct client));
    client1->username = NULL;
    client2->username = NULL;
    client1->next = client2;
    client2->next = NULL;
    client1->username = xmalloc(strlen("bonjour") + 1, sizeof(char));
    client2->username = xmalloc(strlen("b") + 1, sizeof(char));
    strcpy(client1->username, "bonjour");
    strcpy(client2->username, "bonj");

    char *a = "11\n0\nLOGIN\nskkkkkkkrt\n";
    struct message *message = parse_message(a);
    //struct message *message = init_message(REQUEST_MESSAGE_CODE);
    //message->command = "LOGIN";
    struct message *response = handle_login(message, client1);
    cr_assert_eq(strcmp(client1->username, "skkkkkkkrt"), 0);
    cr_assert_eq(strlen(client1->username),strlen(message->payload));
    cr_assert_eq(strcmp(response->payload, "Logged in"), 0);
    cr_assert_eq(response->status_code, RESPONSE_MESSAGE_CODE);

    free(client1->username);
    free(client1);
    free(client2->username);
    free(client2);
    free_partial_message(message);
    free_partial_message(response);

}


Test(commands, login_duplicate_username)
{
    struct client *client1 = xmalloc(1, sizeof(struct client));
    struct client *client2 = xmalloc(1, sizeof(struct client));
    client1->username = NULL;
    client2->username = NULL;
    client1->next = client2;
    client2->next = NULL;
    client1->username = xmalloc(strlen("bonjour") + 1, sizeof(char));
    client2->username = xmalloc(strlen("b") + 1, sizeof(char));
    strcpy(client1->username, "bonjour");
    strcpy(client2->username, "bonj");


    struct message *message = init_message(REQUEST_MESSAGE_CODE);
    message->command = "LOGIN";
    message->payload_size = strlen("skkkkkkkrt") + 1;
    message->payload = xmalloc(message->payload_size ,sizeof(char));
    strcpy(message->payload, "bonjour");
    struct message *response = handle_login(message, client1);
    cr_assert_eq(strcmp(client1->username, "bonjour"), 0);
    cr_assert_eq(strlen(client1->username),strlen(message->payload));
    cr_assert_eq(strcmp(response->payload, "Duplicate username"), 0);
    cr_assert_eq(response->status_code, ERROR_MESSAGE_CODE);
    free(client1->username);
    free(client1);
    free(client2->username);
    free(client2);
    free_partial_message(message);
    free_partial_message(response);
}


Test(commands, login_invalid_username)
{
    struct client *client1 = xmalloc(1, sizeof(struct client));
    struct client *client2 = xmalloc(1, sizeof(struct client));
    client1->username = NULL;
    client2->username = NULL;
    client1->next = client2;
    client2->next = NULL;
    client1->username = xmalloc(strlen("bonjour") + 1, sizeof(char));
    client2->username = xmalloc(strlen("b") + 1, sizeof(char));
    strcpy(client1->username, "bonjour");
    strcpy(client2->username, "bonj");
    struct message *message = init_message(REQUEST_MESSAGE_CODE);
    message->command = "LOGIN";
    message->payload_size = strlen("&ahuia&") + 1;
    message->payload = xmalloc(11 ,sizeof(char));
    strcpy(message->payload, "&ahuia&");
    struct message *response = handle_login(message, client1);
    cr_assert_eq(strcmp(client1->username, "bonjour"), 0);
    cr_assert_eq(strlen(client1->username),strlen(message->payload));
    cr_assert_eq(strcmp(response->payload, "Bad username"), 0);
    cr_assert_eq(response->status_code, ERROR_MESSAGE_CODE);
    free(client1->username);
    free(client1);
    free(client2->username);
    free(client2);
    free_partial_message(message);
    free_partial_message(response);
}*/
/*
Test(commands, list_users, .disabled = true)
{
    struct client *client1 = xmalloc(1, sizeof(struct client));
    struct client *client2 = xmalloc(1, sizeof(struct client));
    client1->username = NULL;
    client2->username = NULL;
    client1->next = client2;
    client2->next = NULL;
    client1->username = xmalloc(strlen("bonjour") + 1, sizeof(char));
    client2->username = xmalloc(strlen("b") + 1, sizeof(char));
    strcpy(client1->username, "bonjour");
    strcpy(client2->username, "bonj");
    struct message *message = init_message(REQUEST_MESSAGE_CODE);
    message->command = "LIST-USERS";
    struct message *response = handle_list_users(message, client1);
    

}*/
/*
Test(commands, send_dm, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}

Test(commands, broadcast, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}

Test(commands, create_room, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}

Test(commands, list_rooms, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}

Test(commands, join_room, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}

Test(commands, leave_room, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}

Test(commands, send_room, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}

Test(commands, delete_room, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}

Test(commands, profile, .disabled = true)
{
    // TODO: Implement this test (then delete `.disabled = true`)
    cr_assert(true);
}
*/
