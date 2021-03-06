#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

enum message_status_code
{
    REQUEST_MESSAGE_CODE = 0,
    RESPONSE_MESSAGE_CODE = 1,
    NOTIFICATION_MESSAGE_CODE = 2,
    ERROR_MESSAGE_CODE = 3,
};

#define MISSING_PARAMETER_RETURN(PAYOAD_COMMAND)                               \
    struct send_pool *sp = xmalloc(1, sizeof(struct send_pool));               \
    struct message *error_message = init_message(ERROR_MESSAGE_CODE);          \
    error_message->payload = strdup("Missing parameter\n");                    \
    error_message->command = strdup((PAYOAD_COMMAND));                         \
    error_message->payload_size = strlen(error_message->payload);              \
                                                                               \
    sp->nb_msg = 1;                                                            \
    sp->msg = xmalloc(1, sizeof(struct message));                              \
    sp->clients_sockets = xmalloc(1, sizeof(int));                             \
    *sp->msg = error_message;                                                  \
    *sp->clients_sockets = client->client_socket;                              \
                                                                               \
    return sp;

/**
 * @brief Structure to store the parsed message
 *
 */
struct message
{
    uint64_t payload_size; // size of the payload
    uint8_t status_code : 2; // enum message_status_code (aligned on 2 bits)
    char *command; // command name
    uint64_t nb_parameters; // number of parameters
    struct command_parameter *command_parameters; // array of parameters
    char *payload; // payload
};

/**
 * @brief Initializes a struct message with the param status code affected
 *
 *
 * @param status_code
 * @return struct message*
 */
struct message *init_message(uint8_t status_code);

/**
 * @brief Structure to store a command parameter
 *
 */
struct command_parameter
{
    char *key; // parameter key
    void *value; // parameter value
};

/**
 * @brief Take a message as a string
 * and parse it into a message struct
 *
 * @param request_string The message as a string
 * @return struct message, NULL if error or invalid message
 */
struct message *parse_message(const char *request_string);

/**
 * @brief Free the memory allocated by a struct message
 *
 * @warning MUST NOT be used on incomplete struct message
 *
 * @param message The struct message to free
 * @return void
 */
void free_message(struct message *message);

/**
 * @brief Like `free_message`, but disalocate a non-valid struct message
 * @note This function is less efficient than `free_message`
 *
 * @param message The struct message to free
 * @return void
 */
void free_partial_message(struct message *message);

/**
 * @brief Get the next key value object
 *
 * @warning This function modifies the input string
 *
 * @param params_start The start of the parameters
 * @param key The key of the object, NULL if no key
 * @param value The value of the object, NULL if no value
 * @return offset to the next object, 0 if no more object
 */
int get_message_next_parameter_kv(char *params_start, char **key, char **value);

/**
 * @brief Takes a message struct and serializes it
 * to a string
 *
 * @param message The message as a message struct
 *
 * @return char pointer
 */
char *compose_message(const struct message *message);

#endif /* MESSAGE_H */
