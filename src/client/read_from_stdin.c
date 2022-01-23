#include "read_from_stdin.h"
#include "xalloc.h"
#include "my_itoa.h"
#include "message.h"
#include "safe_io.h"
#include "client_read.h"
#include "init_socket.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int server_socket;

int command_is_valid(char *a)
{
    if (a == NULL)
        return 0;
    if (strcmp(a, "LOGIN") == 0)
        return 1;
    if (strcmp(a, "PING") == 0)
        return 1;
    if (strcmp(a, "LIST-USERS") == 0)
        return 1;
    if (strcmp(a, "SEND-DM") == 0)
        return 1;
    if (strcmp(a, "BROADCAST") == 0)
        return 1;
    return 0;
}
static int regular_payload(struct message *message, int server_socket)
{
    char payload[DEFAULT_BUFFER_SIZE];
    printf("Payload:\n");
    fgets(payload, DEFAULT_BUFFER_SIZE, stdin); // read the payload we went to send
    payload[strcspn(payload, "\n")] = 0;    // parse the newline
    message->payload = xmalloc(strlen(payload) + 1, sizeof(char)); // allocating payload memory in struct message
    if (strcmp(payload, "/quit") == 0) // If the payload is /quit
    {
        free_partial_message(message); //free the message and exit
        return 0;
    }
    else
    {
        strcpy(message->payload, payload); // store the payload in struct message
        message->payload_size = strlen(payload); // Store the payload size in struct message
        safe_send(server_socket, compose_message(message), strlen(payload) + 1,
                  MSG_EOR); // send the serialized message to the server :WIP:
        return 1;
    }

}
static void looping_payload(struct message *message, int server_socket)
{
    while((regular_payload(message, server_socket) != 0));
}
static void command_send_dm(struct message *message, int server_socket)
{
    char parameters[DEFAULT_BUFFER_SIZE];
    printf("Parameters:\n");
    fgets(parameters, DEFAULT_BUFFER_SIZE, stdin);
    message->command_parameters = xmalloc(message->nb_parameters, sizeof(struct command_parameter));
    while(strcmp(parameters, "\n") != 0)
    {
        if((strncmp(parameters, "User=", 5)!= 0) || (strlen(parameters) == 6 && strncmp(parameters, "User=", 5) == 0))
            fprintf(stderr, "Invalid parameter\n");
        else
        {
            parameters[strcspn(parameters, "\n")] = '\0';
            message->command_parameters[message->nb_parameters].key = xmalloc(5, sizeof(char));
            message->command_parameters[message->nb_parameters].value = xmalloc(strlen(parameters), sizeof(char));
            strcpy(message->command_parameters[message->nb_parameters].key, "User\0");
            strcpy(message->command_parameters[message->nb_parameters].value, parameters + 5);
            message->nb_parameters++;
        }
        fgets(parameters, DEFAULT_BUFFER_SIZE, stdin);
    }
    looping_payload(message, server_socket);
}
static void treat_commands(struct message *message, char *command, int server_socket)
{
    if(strcmp(command, "PING") == 0 || strcmp(command, "LIST-USERS\n") == 0 || strcmp(command, "LOGIN\n") == 0)
        regular_payload(message, server_socket);
    if(strcmp(command, "SEND-DM") == 0)
        command_send_dm(message, server_socket);
    if(strcmp(command, "BROADCAST") == 0)
        looping_payload(message, server_socket);
}

#ifdef DEBUG
#define READ_FROM_STDIN_LOOP 0
#else
#define READ_FROM_STDIN_LOOP 1
#endif

void read_from_stdin(int server_socket)
{
    do
    {
        struct message *message = init_message(REQUEST_MESSAGE_CODE); // Initializing struct message
        if (!message)   //If allocating fails
        {
            fprintf(stderr, "Error while allocating memory\n"); //Exiting properly
            close(server_socket);
            exit(1);
        }        
        //char * command = xmalloc(DEFAULT_BUFFER_SIZE, sizeof(char));
        //safe_read(STDIN_FILENO,(void **) &command);

        char command[DEFAULT_BUFFER_SIZE];
        fprintf(stdout, "Command:\n");
        fgets(command, DEFAULT_BUFFER_SIZE, stdin); //Get user input
        command[strcspn(command, "\n")] = 0;    // parse the newline
        if(command_is_valid(command)) //If the command is valid
        {
            message->command = xmalloc(strlen(command) + 1, sizeof(char));
            strcpy(message->command, command);
            treat_commands(message, command, server_socket);    
        }
        else if (command == NULL) //If the entry isn't valid
        {
            free_partial_message(message);
            fprintf(stderr, "Error while reading STDIN data\n");
            close(server_socket);
            exit(1);
        }
        else // If the command isn't valid
        {
            fprintf(stderr, "Invalid command\n");
            free_partial_message(message);
        }
    } while (READ_FROM_STDIN_LOOP);
}

void *read_from_stdin_thread(void *none)
{
    (void) none;
    int s = server_socket;
    read_from_stdin(s);
    return NULL;
}
