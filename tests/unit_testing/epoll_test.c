#include <criterion/criterion.h>
#include <unistd.h>

#include "init_socket.h"

Test(socket, init_socket)
{
    int fd = setup_server_socket("127.0.0.1", "8090");
    cr_assert_neq(fd, -1);
    close(fd);
}

// TODO Handle voluntary crash
// Test(socket, init_socket_with_bad_ip)
// {
//     int fd = setup_socket("cmonchouchouparlavitreellemefaiscoucou", "8080");
//     cr_assert_eq(fd, -1);
// }

Test(epoll, server_main_c, .disabled = true)
{
    // TODO Implement this test with a mock server
    cr_assert(true);
}
