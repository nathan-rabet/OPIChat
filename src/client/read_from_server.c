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
    while (1)
    {
        struct message *m = safe_recv(server_sockfd, 0, false);
        if (m == NULL)
        {
            while ((server_sockfd = setup_client_socket(ip, port)) == -1)
            {
                write_error(
                    "Impossible to connect to server %s:%s, retrying... : %s",
                    ip, port, strerror(errno));
                sleep(1);
            }
        }

        else if (m == NULL)
        {
            write_warning("Receving corrupted message from server");
        }

        else
        {
            switch (m->status_code)
            {
            case RESPONSE_MESSAGE_CODE:
                if (strcmp(m->payload, "") != 0)
                    fprintf(stdout, "< %s\n", m->payload);
                break;

            case ERROR_MESSAGE_CODE:
                fprintf(stdout, "! %s\n", m->payload);
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
