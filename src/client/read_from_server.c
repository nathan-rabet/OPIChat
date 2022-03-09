#include "read_from_server.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "init_socket.h"
#include "logger.h"
#include "message.h"
#include "safe_io.h"

extern int server_socket;
extern char *ip;
extern char *port;

void *read_from_server_thread(void *none)
{
    (void)none;
    int server_sockfd = server_socket;
    while (true)
    {
        struct message *m = safe_recv(server_sockfd, 0, false);
        if (m == NULL)
        {
            do
            {
                close(server_socket);
                server_sockfd = setup_client_socket(ip, port);
                write_error(
                    "Impossible to connect to server %s:%s, retrying... : %s",
                    ip, port, strerror(errno));
                sleep(1);
            } while (server_socket == -1);
        }
        else
        {
            switch (m->status_code)
            {
            case RESPONSE_MESSAGE_CODE:
                if (strcmp(m->payload, "") != 0)
                    fprintf(stdout, "< %s", m->payload);
                break;

            case ERROR_MESSAGE_CODE:
                fprintf(stdout, "! %s", m->payload);
                break;

            case NOTIFICATION_MESSAGE_CODE:
                if (strcmp(m->command, "SEND-DM") == 0
                    || strcmp(m->command, "BROADCAST") == 0)
                {
                    uint64_t i = 0;
                    while (strcmp(m->command_parameters[i].key, "From") != 0
                           && i < m->nb_parameters)
                        i++;
                    fprintf(stdout, "From %s: %s\n",
                            (char *)m->command_parameters[i].value, m->payload);
                }

                if (strcmp(m->command, "SEND-ROOM") == 0)
                {
                    uint64_t i_from = 0;
                    uint64_t i_room = 0;
                    while (strcmp(m->command_parameters[i_from].key, "From")
                               != 0
                           && i_from < m->nb_parameters)
                        i_from++;
                    while (strcmp(m->command_parameters[i_room].key, "Room")
                               != 0
                           && i_room < m->nb_parameters)
                        i_room++;
                    fprintf(stdout, "From %s@%s: %s\n",
                            (char *)m->command_parameters[i_from].value,
                            (char *)m->command_parameters[i_room].value,
                            m->payload);
                }
                break;

            default:
                write_warning(
                    "Receving corrupted message type from server '%d'",
                    m->status_code);
                break;
            }

            free_message(m);
        }
    }
}
