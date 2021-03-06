#include "read_from_stdin.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client_read.h"
#include "init_socket.h"
#include "logger.h"
#include "message.h"
#include "my_itoa.h"
#include "safe_io.h"
#include "xalloc.h"

extern int server_socket;

struct command_parameters
{
    char *command; // NULL if invalid
    uint8_t nb_parameters;
    char **parameters_name;
    bool loop_payload;
};

// TODO : Do not use the heap, use a fixed size array instead
static struct command_parameters *
get_command_parameters_info(char *command_string)
{
    struct command_parameters *cmd_params =
        xcalloc(1, sizeof(struct command_parameters));
    if (command_string != NULL)
    {
        if (strcmp(command_string, "LOGIN") == 0)
            cmd_params->command = "LOGIN";
        else if (strcmp(command_string, "PING") == 0)
            cmd_params->command = "PING";
        else if (strcmp(command_string, "LIST-USERS") == 0)
            cmd_params->command = "LIST-USERS";
        else if (strcmp(command_string, "BROADCAST") == 0)
        {
            cmd_params->loop_payload = true;
            cmd_params->command = "BROADCAST";
        }
        else if (strcmp(command_string, "SEND-DM") == 0)
        {
            cmd_params->loop_payload = true;
            cmd_params->command = "SEND-DM";
            cmd_params->nb_parameters = 1;
            cmd_params->parameters_name = xmalloc(1, sizeof(char **));
            cmd_params->parameters_name[0] = strdup("User");
        }

        else if (strcmp(command_string, "CREATE-ROOM") == 0)
            cmd_params->command = "CREATE-ROOM";
        else if (strcmp(command_string, "LIST-ROOMS") == 0)
            cmd_params->command = "LIST-ROOMS";
        else if (strcmp(command_string, "JOIN-ROOM") == 0)
            cmd_params->command = "JOIN-ROOM";
        else if (strcmp(command_string, "LEAVE-ROOM") == 0)
            cmd_params->command = "LEAVE-ROOM";
        else if (strcmp(command_string, "SEND-ROOM") == 0)
        {
            cmd_params->loop_payload = true;
            cmd_params->command = "SEND-ROOM";
            cmd_params->nb_parameters = 1;
            cmd_params->parameters_name = xmalloc(1, sizeof(char **));
            cmd_params->parameters_name[0] = strdup("Room");
        }
        else if (strcmp(command_string, "DELETE-ROOM") == 0)
            cmd_params->command = "DELETE-ROOM";
        else if (strcmp(command_string, "PROFILE") == 0)
            cmd_params->command = "PROFILE";
    }
    return cmd_params;
}

static void free_command_parameters_info(struct command_parameters *cmd_params)
{
    for (uint8_t i = 0; i < cmd_params->nb_parameters; i++)
        free(cmd_params->parameters_name[i]);
    free(cmd_params->parameters_name);
    free(cmd_params);
}

void read_from_stdin(int server_socket)
{
    do
    {
        struct message *message =
            init_message(REQUEST_MESSAGE_CODE); // Initializing struct message
        if (!message)
        {
            close(server_socket);
            raise_panic(EXIT_FAILURE,
                        "Error while allocating memory\n"); // Exiting properly
        }

        char command[DEFAULT_BUFFER_SIZE];
        fprintf(stdout, "Command:\n");
        if (!fgets(command, DEFAULT_BUFFER_SIZE,
                   stdin)) // Get user input, stop on EOF
        {
            free_message(message);
            logger_close();
            exit(EXIT_SUCCESS);
        }

        command[strcspn(command, "\n")] = 0; // parse the newline

        struct command_parameters *cmd_params =
            get_command_parameters_info(command);

        if (cmd_params->command) // If the command is valid
        {
            message->command = xmalloc(strlen(command) + 1, sizeof(char));
            message->nb_parameters = 0;

            strcpy(message->command, command);

            if (cmd_params->nb_parameters > 0)
            {
                char parameters[DEFAULT_BUFFER_SIZE] = { 0 };
                uint8_t entered_params = 0;

                fprintf(stdout, "Parameters:\n");
                while (1)
                {
                    if (!fgets(parameters, DEFAULT_BUFFER_SIZE, stdin))
                    {
                        free_message(message);
                        free_command_parameters_info(cmd_params);
                        logger_close();
                        exit(EXIT_SUCCESS);
                    }

                    if (parameters[0] == '\n')
                        break;

                    char *key;
                    char *value;

                    // Get a "key=value" pair using
                    // get_message_next_parameter_kv (witch also works in this
                    // case)
                    get_message_next_parameter_kv(parameters, &key, &value);
                    bool parameterFound = false;
                    for (uint8_t i = 0; i < cmd_params->nb_parameters; i++)
                    {
                        if (key
                            && strcmp(key, cmd_params->parameters_name[i]) == 0)
                        {
                            parameterFound = true;
                            message->command_parameters =
                                xrealloc(message->command_parameters,
                                         message->nb_parameters + 1,
                                         sizeof(struct command_parameter));

                            message->command_parameters[entered_params].key =
                                strdup(key);
                            message->command_parameters[entered_params].value =
                                strdup(value);

                            message->nb_parameters = ++entered_params;
                            break;
                        }
                    }

                    if (!parameterFound)
                    {
                        fprintf(stderr, "Invalid parameter\n");
                        continue;
                    }
                }
            }
        }
        else
        {
            fprintf(stderr, "Invalid command\n");
            free_partial_message(message);
            free_command_parameters_info(cmd_params);
            continue;
        }

        do
        {
            char *payload = command; // Reuse the command buffer
            fprintf(stdout, "Payload:\n");
            if (!fgets(payload, DEFAULT_BUFFER_SIZE, stdin)) // Get user input
            {
                free_message(message);
                free_command_parameters_info(cmd_params);
                logger_close();
                exit(EXIT_SUCCESS);
            }
            payload[strcspn(payload, "\n")] = 0; // Eliminate the newline

            if (strcmp(payload, "/quit") != 0)
            {
                message->payload = strdup(payload);
                message->payload_size = strlen(payload);

                char *serialized_message = compose_message(message);
                if (serialized_message)
                {
                    safe_send(server_socket, serialized_message,
                              strlen(serialized_message), MSG_EOR);
                    free(serialized_message);
                }
                else
                {
                    close(server_socket);
                    raise_panic(EXIT_FAILURE,
                                "Error while serializing message\n");
                }
            }
            else
                cmd_params->loop_payload = false;
        } while (cmd_params->loop_payload);

        free_command_parameters_info(cmd_params);
        free_message(message);

    } while (1);
}

void *read_from_stdin_thread(void *none)
{
    (void)none;
    int s = server_socket;
    read_from_stdin(s);
    return NULL;
}
