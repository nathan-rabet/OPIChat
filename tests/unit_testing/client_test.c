#include <criterion/criterion.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"

Test(client, add_client__remove_client)
{
    struct client *client1 = NULL;
    struct client *client2 = NULL;
    struct sockaddr addr = {
        .sa_family = AF_INET,
    };

    client1 = add_client(client1, 1, addr, sizeof(addr));

    cr_assert_not_null(client1);
    cr_assert_eq(client1->client_socket, 1);
    cr_assert_eq(client1->client_sockaddr->ss_family, AF_INET, "Family is %d",
                 client1->client_sockaddr->ss_family);

    client2 = add_client(client1, 2, addr, sizeof(addr));

    cr_assert_not_null(client2);
    cr_assert_eq(client2->client_socket, 2);
    cr_assert_eq(client2->client_sockaddr->ss_family, AF_INET, "Family is %d",
                 client2->client_sockaddr->ss_family);

    cr_assert_eq(client2->next, client1,
                 "client2->next is %p and client1 is %p", client2->next,
                 client1);

    client2 = remove_client(client2, 1);
    client2 = remove_client(client2, 2);

    cr_assert_null(client2);
}

Test(client, find_client)
{
    struct client *client = NULL;
    struct client *found_client = NULL;
    struct sockaddr addr = {
        .sa_family = AF_INET,
    };

    client = add_client(NULL, 1, addr, sizeof(addr));
    client = add_client(client, 2, addr, sizeof(addr));

    found_client = find_client(client, 1);
    cr_assert_not_null(found_client);
    cr_assert_eq(found_client->client_socket, 1);
    cr_assert_eq(found_client->client_sockaddr->ss_family, AF_INET);

    found_client = find_client(client, 2);
    cr_assert_not_null(found_client);
    cr_assert_eq(found_client->client_socket, 2);
    cr_assert_eq(found_client->client_sockaddr->ss_family, AF_INET);

    client = remove_client(client, 1);
    client = remove_client(client, 2);

    cr_assert_null(client, "Found client is not null: %p", found_client);
}
