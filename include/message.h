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
int message_next_key_value(char *params_start, char **key, char **value);

#endif /* MESSAGE_H */
