#ifndef COMMANDS_H
#define COMMANDS_H

#include "client.h"
#include "message.h"

struct send_pool
{
    uint8_t nb_msg;
    int *clients_sockets;
    struct message **msg;
};

struct send_pool *handle_ping(struct message *msg, struct client *client);

struct send_pool *handle_login(struct message *msg, struct client *client);

struct send_pool *handle_list_users(struct message *msg, struct client *client);

struct send_pool *handle_send_dm(struct message *msg, struct client *client);

struct send_pool *handle_broadcast(struct message *msg, struct client *client);

struct send_pool *handle_create_room(struct message *msg,
                                     struct client *client);

struct send_pool *handle_list_rooms(struct message *msg);

struct send_pool *handle_join_room(struct message *msg, struct client *client);

struct send_pool *handle_leave_room(struct message *msg, struct client *client);

struct send_pool *handle_send_room(struct message *msg, struct client *client);

struct send_pool *handle_delete_room(struct message *msg,
                                     struct client *client);

struct send_pool *handle_profile(struct message *msg, struct client *client);

int username_not_duplicate(char *name, struct client *clients,
                           struct client *me);

#endif /* COMMANDS_H */
