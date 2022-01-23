#ifndef READ_FROM_STDIN_H
#define READ_FROM_STDIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>




/**
 * @brief Reads request from stdin and sends it so server
 * 
 * @param server_socket 
 */
void read_from_stdin(int server_socket);

/**
 * @brief Checks whether the param is a valid command
 * 
 * @param command
 * @return int 
 */
int command_is_valid(char *command);

void *read_from_stdin_thread(void *server_socket);



#endif /* READ_FROM_STDIN_H */

