#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

enum log_cp_on_file
{
    LOG_CP_ON_FILE_NONE = 0
};

/**
 * @brief Init the logger system
 *
 * @param path The path of the file to log to
 */
void logger_init(const char *path);

/**
 * @brief Get the logger system *FILE
 *
 * @return The logger *FILE
 */
FILE *get_logger_file();

/**
 * @brief Log a message
 *
 * @note This function supports formatted string (like printf)
 *
 * @param banner The banner to display before the message
 * @param cp_on_file Copy the message to the file
 * (ignored if set on `LOG_CP_ON_FILE_NONE`)
 * @param str The message to log
 */
void write_logger(const char *banner, FILE *cp_on_file, const char *str, ...);

/**
 * @brief Log an info message
 *
 * @note This function supports formatted string (like printf)
 *
 * @param msg The message to log
 */
void write_info(const char *msg, ...);

/**
 * @brief Log an error message
 *
 * @note This function supports formatted string (like printf)
 *
 * @param msg The message to log
 */
void write_warning(const char *msg, ...);

/**
 * @brief Log an error message
 *
 * @note This function supports formatted string (like printf)
 *
 * @param msg The message to log
 */
void write_error(const char *msg, ...);

/**
 * @brief Log a CRITICAL error message & exit the program
 *
 * @note This function supports formatted string (like printf)
 *
 * @param exitCode The exit code to use
 * @param msg The error to log
 */
void raise_panic(int exitCode, const char *msg, ...);

/**
 * @brief Close the logger system properly
 *
 */
void logger_close();

#endif /* LOGGER_H */
