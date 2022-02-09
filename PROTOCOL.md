# Protocol

## Introduction

In OPIChat, clients can send messages to the server. We will call such messages `requests`.

These requests contain a command, indicating the action that needs to be performed by the server.

The server will reply with another message called either `response` or `error` depending on the completion of the request. It provides information about the status of the performed action.

A server might also send unsolicited responses to connected clients to inform them of an event (for example, notify that a client just received a message from another client). Such messages will be called `notifications`.

The OPIChat protocol is inspired by HTTP and IRC. We introduce you to a simplified version of these
protocols so you do not lose time reading complex specifications.

## Syntax

Requests, responses, notifications and errors in *OPIChat* all follow the same syntax.

They are composed of five main components: payload size, status code, command, command parameters and payload:

- The payload size is the number of bytes of the payload.
- The status code is an integer indicating the type of the message or the possible errors. Every
integer has its own meaning:
  - A status code of 0 indicates that the message is a `request`.
  - A status code of 1 indicates that it is a successful `response`.
  - A status code of 2 indicates a notification from the server to a client (for example a message from a client to another).
  - A status code of 3 indicates that the message is a response to notify an error while processing a request, such message is called an `error`.
- The command is a string identifying the action performed by the server.
- The command parameters specify special requirements for a command (for example the username of a client you might want to send a message to). There can be any number of parameters in a message, each command as its own required parameters documented in the appropriate sections. Each parameter consists of a key/value pair with the following syntax: `key=value`. Parameters can be in any order in the message.

Each of theses elements are separated by a newline and there can be any number of parameters. The
end of command parameters is marked by an empty line, which therefore represents the start of the
payload.

For example the following is a request sent by a user named `ING1` asking the server to send the message
`2022` to the user `acu`:

> 4\$\
> 0\$\
> SEND-DM\$\
> User=acu\$\
> $\
> 2022

In this example and all the others in this document, the <kbd>$</kbd> character corresponds to a newline.

Such request will cause the server to send a response to the original client indicating the status of the operation. Upon successful completion, the response will look like this:

> 0\$\
> 1\$\
> SEND-DM\$\
> User=acu\$\
> \$

While processing the request, the server will also generate a notification for user acu to notify them
of a message:

> 4\$\
> 2\$\
> SEND-DM\$\
> User=acu\$\
> From=ING1\$\
> \$\
> 2022

## Core features

This section defines the implemented commands.

> Unless stated otherwise and if not specified, payloads and parameters can be ignored.

### PING

The PING command does not require any payload in the request. The server must simply respond with `PONG$` in the payload.

#### Parameters

The `PING` command does not require any parameter.

#### Error codes

No specific error codes are defined for the `PING` command.

#### Examples

Request

> 0\$\
> 0\$\
> PING\$\
> \$

Response

> 5\$\
> 1\$\
> PING\$\
> \$\
> PONG\$

### LOGIN

The `LOGIN` command is used by a client to assign itself a user name.

The user name is sent in the payload of the request and consists solely of alpha-numerical characters.

Upon succesfull completion, the server must sendLogged in$in the response.

#### Parameters

The `LOGIN` command does not take any parameters.

#### Error codes

Bad username

This error is sent when the provided user name is invalid (i.e. it contains non alpha-numerical characters or empty user name).

- Payload:`Bad username$`

Duplicate username

This error is sent when the provided user name is already used by another client.

- Payload:`Duplicate username$`

#### Examples

Request

> 3\$\
> 0\$\
> LOGIN\$\
> \$\
> acu

Response

> 10\$\
> 1\$\
> LOGIN\$\
> \$\
> Logged in\$


### LIST-USERS

The `LIST-USERS` command lists all currently connected and logged in users.

Users are outputted in the payload of the response by outputting each user name followed by a newline. Client that are not logged in are not listed.

User names are ordered by connection date in ascending order.

#### Parameters

The `LIST-USERS` command does not require any parameter.

#### Error codes

No specific error codes are defined for the `LIST-USERS` command.

#### Examples

Request

> 0\$\
> 0\$\
> LIST-USERS\$\
> \$

Response

> 15\$\
> 1\$\
> LIST-USERS\$\
> \$\
> acu\$\
> Hoppy\$\
> ING1\$


### SEND-DM.

The `SEND-DM` command asks the server to send a direct message to the user specified in parameter.
Such direct message is represented by a notification containing the same payload as the request.

Upon completion, the server must send a response with an empty payload.

#### Parameters

User

Represents the user name of the recipient of the message. Required in request, response and notification.

From

Represents the user name of the sender of the message. Required in notification.

If the sender is not logged in, the `From` parameter must take the value `<Anonymous>`.

#### Error codes

User not found

This error is sent when the specified user could not be found.

- Payload:`User not found$`

#### Client side

Upon receiving a `SEND-DM` notification from the server, a client must log the message on `stdout` with the following format:

`From <user>: <payload>$`

By replacing `<user>` by the user name of the sender and `<payload>` by the payload of the notification.

#### Examples

`ING1` sends the direct message `2022` to `acu`:

Request

From `ING1` to the server

> 4\$\
> 0\$\
> SEND-DM\$\
> User=acu\$\
> \$\
> 2022

Response

From the server toING1

> 0\$\
> 1\$\
> SEND-DM\$\
> User=acu\$\
> \$

Notification

From the server toacu

> 4\$\
> 2\$\
> SEND-DM\$\
> User=acu\$\
> From=ING1\$\
> \$\
> 2022

### BROADCAST

The broadcast command is used to send a direct message to every user currently connected, logged in or not, excluding the original sender. As for theSEND-DM, the message is represented by a notification containing the same payload as the request.

Upon completion, the server must send a response with an empty payload.

#### Parameters

From

Represents the user name of the sender of the message. Required in notification.

If the sender is not logged in, the `From` parameter must take the value `<Anonymous>`.

#### Error codes

No specific error codes are defined for the `BROADCAST` command.

#### Client side

Upon receiving a `BROADCAST` notification from the server, a client must log the message on `stdout` with the following format:

`From <user>: <payload>$`

By replacing `<user>` by the user name of the sender and `<payload>` by the payload of the notification.

#### Examples

Request

> 4\$\
> 0\$\
> BROADCAST\$\
> \$\
> 2022

Response

> 0\$\
> 1\$\
> BROADCAST\$\
> \$

Notification

> 4\$\
> 2\$\
> BROADCAST\$\
> From=ING1\$\
> \$\
> 2022


## Additional features

This section defines more advanced commands that will for example allow you to create chat rooms.

### CREATE-ROOM.

The `CREATE-ROOM` command is used by a client to create a room.

The room name is sent in the payload of the request and consists solely of alpha-numericalcharacters.

The user making the request is marked as the owner of the room.

Upon completion, the server must send a response with `Room created$` in the payload.

#### Parameters

The `CREATE-ROOM` command does not take any parameters.

#### Error codes

Bad name

This error is sent when the provided room name is invalid (i.e. it contains non alpha-numerical characters or empty room name).

- Payload:`Bad room name$`

Duplicate name

This error is sent when trying to create a room when another room already owns the same name.

- Payload:`Duplicate room name$`

#### Examples

Request

> 8\$\
> 0\$\
> CREATE-ROOM\$\
> \$\
> FlagRoom


Response

> 13\$\
> 1\$\
> CREATE-ROOM\$\
> \$\
> Room created$

### LIST-ROOMS

The `LIST-ROOMS` command lists all active rooms.

Rooms are outputted in the payload of the response by outputting each room name suffixed by a newline.

Room names are ordered by creation date in ascending order.

#### Parameters

The `LIST-ROOMS` command does not require any parameter.

#### Error codes

No specific error codes are defined for the `LIST-ROOMS` command.

#### Examples

Request

> 0\$\
> 0\$\
> LIST-ROOMS\$\
> \$

Response

> 24\$\
> 1\$\
> LIST-ROOMS\$\
> \$\
> CISCO\$\
> LABSR\$\
> MIDLAB\$\
> SM14\$


### JOIN-ROOM

The `JOIN-ROOM` command is used by a client to subscribe to a room.

A user subscribed to a room will receive all messages sent to this room.

The room to join is indicated in the payload of the request.

Upon completion, the server must send `Room joined$` in the payload.

#### Parameters

The `JOIN-ROOM` command does not require any parameter.

#### Error codes

Room not found

This error is sent when the specified room could not be found on the server.

- Payload:`Room not found$`

#### Examples

Request

> 8\$\
> 0\$\
> JOIN-ROOM\$\
> \$\
> FlagRoom

Response

> 12\$\
> 1\$\
> JOIN-ROOM\$\
> \$\
> Room joined\$

### LEAVE-ROOM

The `LEAVE-ROOM` command is used by a client to unsubscribe from a room.

The room to leave is indicated in the payload of the request.

Upon completion, the server must sendRoom left$in the payload.

#### Parameters

The `LEAVE-ROOM` command does not require any parameter.

#### Error codes

Room not found

This error is sent when the specified room could not be found on the server.

- Payload:`Room not found$`

#### Examples

Request

> 8\$\
> 0\$\
> LEAVE-ROOM\$\
> \$\
> FlagRoom

Response

> 10\$\
> 1\$\
> LEAVE-ROOM\$\
> \$\
> Room left\$

### SEND-ROOM

The `SEND-ROOM` command asks the server to send a message to a room and therefore, all users who
joined it except the original sender.

Upon completion, the server must send a response with an empty payload.

#### Parameters

Room

Represents the destination room name. Required in request, response and notification.

From

Represents the user name of the sender of the message. Required in notification.

If the sender is not logged in, the `From` parameter must take the value `<Anonymous>`.

#### Error codes

Room not found

This error is sent when the specified room could not be found on the server.

- Payload:`Room not found$`

#### Client side

Upon receiving a SEND-ROOM notification from the server, a client must log the message on `stdout` with the following format:

From `<user>@<room>: <payload>$`

By replacing`<user>`by the user name of the sender, `<room>` by the name of the room and `<payload>` by the payload of the notification.

#### Examples

`ING1` sends the message `2022` to the `FlagRoom`:

Request

From `ING1` to the server

> 4\$\
> 0\$\
> SEND-ROOM\$\
> Room=FlagRoom\$\
> \$\
> 2022

Response

From the server to `ING1`

> 0\$\
> 1\$\
> SEND-ROM\$\
> Room=FlagRoom\$\
> \$

Notification

From the server to every user who joined `FlagRoom`

> 4\$\
> 2\$\
> SEND-ROOM\$\
> Room=FlagRoom\$\
> From=ING1\$\
> \$\
> 2022

### DELETE-ROOM

The `DELETE-ROOM` Deletes the room specified in the payload.

The deletion is possible only if the user making the request is the owner of the room.

Upon completion, the server must sendRoom deletedin the payload.

#### Parameters

The `DELETE-ROOM` command does not require any parameter.

#### Error codes

Room not found

This error is sent when the specified room could not be found on the server.

- Payload:`Room not found$`

Unauthorized

This error is sent if the user is not the owner of the specified room

- Payload:`Unauthorized$`

#### Examples

Request

> 8\$\
> 0\$\
> DELETE-ROOM\$\
> \$\
> FlagRoom

Response

> 13\$\
> 1\$\
> DELETE-ROOM\$\
> \$\
> Room deleted\$

### PROFILE

The `PROFILE` command lists different information about the user making the request.

The payload of the response follows the following syntax:

> Username: \<name\>\$\
> IP: \<ip\>\$\
> Rooms:\$\
> \<room\>$*

`<name>` corresponds to the user name of the client making the request.

`<ip>` is the IP address of the client making the request.

`<room>` is the name of a room joined by the client making the request. Every room joined by the user must be listed by room creation date in ascending order and suffixed by a newline.

#### Parameters

The `PROFILE` command does not require any parameter.

#### Error codes

No specific error codes are defined for the `PROFILE` command.

#### Examples

Request

> 0\$\
> 0\$\
> PROFILE\$\
> $

Response

> 50\$\
> 1\$\
> PROFILE\$\
> \$\
> Username: acu\$\
> IP: 127.0.0.1\$\
> Rooms:\$\
> CISCO\$\
> FlagRoom\$
