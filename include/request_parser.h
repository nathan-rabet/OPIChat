#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdint.h>

/**
 * @brief Structure to store the parsed request
 *
 */
struct request
{
    uint64_t payload_size;
    uint8_t status_code;
    char *command;
    uint64_t nb_parameters;
    struct command_parameter *command_parameters;
    char *payload;
};

/**
 * @brief Structure to store a command parameter
 *
 */
struct command_parameter
{
    char *key;
    void *value;
};

/**
 * @brief Take a request as a string
 * and parse it into a request struct
 *
 * @param request_string The request as a string
 * @return struct request
 */
struct request *parse_request(char *request_string);

/**
 * @brief Free the memory allocated by a struct request
 *
 * @param request The struct request to free
 * @return void
 */
void free_request(struct request *request);

#endif /* REQUEST_PARSER_H */
