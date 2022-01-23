#ifndef COMMANDS_H
#define COMMANDS_H

#include "client.h"
#include "message.h"

struct message *handle_ping(struct message *msg, struct client *client);

struct message *handle_login(struct message *msg, struct client *client);

struct message *handle_list_users(struct message *msg);

struct message *handle_send_dm(struct message *msg, struct client *client);

struct message *handle_broadcast(struct message *msg, struct client *client);

struct message *handle_create_room(struct message *msg, struct client *client);

struct message *handle_list_rooms(struct message *msg);

struct message *handle_join_room(struct message *msg, struct client *client);

struct message *handle_leave_room(struct message *msg, struct client *client);

struct message *handle_send_room(struct message *msg, struct client *client);

struct message *handle_delete_room(struct message *msg, struct client *client);

struct message *handle_profile(struct message *msg, struct client *client);

#endif /* COMMANDS_H */