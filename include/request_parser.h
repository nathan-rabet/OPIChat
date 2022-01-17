#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdint.h>
enum
{
    REQUEST_CODE = 0,
    RESPONSE_CODE = 1,
    NOTIFICATION_CODE = 2,
    ERROR_CODE = 3
};

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
 * @return struct request, NULL if error or invalid request
 */
struct request *parse_request(const char *request_string);

/**
 * @brief Free the memory allocated by a struct request
 *
 * @warning MUST NOT be used on incomplete struct request
 *
 * @param request The struct request to free
 * @return void
 */
void free_request(struct request *request);

/**
 * @brief Like `free_request`, but disalocate a non-valid struct request
 * @note This function is less efficient than `free_request`
 *
 * @param request The struct request to free
 * @return void
 */
void free_partial_request(struct request *request);

#endif /* REQUEST_PARSER_H */
