#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "room.h"

int main(void)
{
    struct room *room1 = NULL;
    struct room *room2 = NULL;

    room1 = add_room(room1, "Room 1", 1);
    room2 = add_room(room1, "Room 1", 2);

    room1 = remove_room(room2, "Room 1", 1);

    printf("%s\n", room1->name);
}
