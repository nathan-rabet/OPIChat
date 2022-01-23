#include <criterion/criterion.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "room.h"

Test(room, add_room)
{
    struct room *room1 = NULL;
    struct room *room2 = NULL;

    room1 = add_room(room1, "Room 1", 1);
    cr_assert_eq(errno, ROOM_ERROR_NONE, "errno is %d", errno);

    cr_assert_not_null(room1);
    cr_assert_eq(room1->owner_socket, 1);
    cr_assert_str_eq(room1->name, "Room 1");
    cr_assert_eq(room1->next, NULL);

    room2 = add_room(room1, "Room 2", 2);
    cr_assert_eq(errno, ROOM_ERROR_NONE);

    cr_assert_not_null(room2);
    cr_assert_eq(room2->owner_socket, 2);
    cr_assert_str_eq(room2->name, "Room 2");

    cr_assert_eq(room2->next, room1);

    room2 = remove_room(room2, "Room 2", 2);
    room2 = remove_room(room2, "Room 1", 1);

    cr_assert_null(room2);
}

Test(room, add_room_non_alphanumeric)
{
    struct room *room = NULL;

    room = add_room(room, "Bad\x01name", 1);
    cr_assert_eq(errno, ROOM_ERROR_BAD_NAME, "errno is %d", errno);

    cr_assert_null(room);
}

Test(room, add_room_duplicate)
{
    struct room *room1 = NULL;
    struct room *room2 = NULL;

    room1 = add_room(room1, "Room 1", 1);
    room2 = add_room(room1, "Room 1", 2);
    cr_assert_eq(errno, ROOM_ERROR_DUPPLICATE_NAME);
    cr_assert_eq(room1, room2);

    room1 = remove_room(room2, "Room 1", 1);

    cr_assert_null(room1);
}

Test(room, find_room)
{
    struct room *room = NULL;
    struct room *found_room = NULL;

    room = add_room(room, "Room 1", 1);
    room = add_room(room, "Room 2", 2);

    found_room = find_room(room, "Room 1");
    cr_assert_not_null(found_room);
    cr_assert_str_eq(found_room->name, "Room 1");
    cr_assert_eq(errno, ROOM_ERROR_NONE);

    found_room = find_room(room, "Room 2");
    cr_assert_not_null(found_room);
    cr_assert_str_eq(found_room->name, "Room 2");
    cr_assert_eq(errno, ROOM_ERROR_NONE);

    room = remove_room(room, "Room 1", 1);
    room = remove_room(room, "Room 2", 2);

    cr_assert_null(room, "Found room is not null: %p", room);
}

Test(room, find_room_no_found)
{
    struct room *room = NULL;
    struct room *found_room = NULL;

    room = add_room(room, "Room 1", 1);
    room = add_room(room, "Room 2", 2);

    found_room = find_room(room, "Wrong name");
    cr_assert_null(found_room);
    cr_assert_eq(errno, ROOM_ERROR_NOT_FOUND);

    room = remove_room(room, "Room 1", 1);
    room = remove_room(room, "Room 2", 2);

    cr_assert_null(room, "Found room is not null: %p", room);
}

Test(room, remove_room)
{
    struct room *room = NULL;
    struct room *new_rooms = NULL;

    room = add_room(room, "Room 1", 1);
    room = add_room(room, "Room 2", 2);

    new_rooms = remove_room(room, "Room 1", 1);
    cr_assert_not_null(new_rooms);
    cr_assert_str_eq(new_rooms->name, "Room 2");
    cr_assert_eq(errno, ROOM_ERROR_NONE);

    new_rooms = remove_room(room, "Room 2", 2);
    cr_assert_null(new_rooms);
    cr_assert_eq(errno, ROOM_ERROR_NONE);

    cr_assert_null(new_rooms, "Found room is not null: %p", new_rooms);
}

Test(room, remove_room_no_found)
{
    struct room *room = NULL;

    room = add_room(room, "Room 1", 1);
    room = add_room(room, "Room 2", 2);

    room = remove_room(room, "Wrong name", 1);
    cr_assert_not_null(room);
    cr_assert_eq(errno, ROOM_ERROR_NOT_FOUND);

    room = remove_room(room, "Room 1", 1);
    room = remove_room(room, "Room 2", 2);

    cr_assert_null(room, "Found room is not null: %p", room);
}
