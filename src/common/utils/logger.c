#include "logger.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

FILE *logger_file;

void logger_init(const char *path)
{
    logger_file = fopen(path, "w");
    if (logger_file == NULL)
    {
        fprintf(stderr, "[LOGGER] Could not open file %s\n", path);
        exit(EXIT_FAILURE);
    }
}

FILE *get_logger_file()
{
    return logger_file;
}

/**
 * @brief General function to log a message
 *
 * @param str The message to log
 * @param banner The banner to display before the message
 * @param cp_on_file Copy the message to the file
 * (ignored if set on `LOG_CP_ON_FILE_NONE`)
 * @param args The arguments of the message
 */
static void __write_logger(const char *str, const char *banner,
                           FILE *cp_on_file, va_list args)
{
    // Get current time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char time_str[64];

    // Format time
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &tm);

#ifdef DEBUG
    // Write to file
    fprintf(logger_file, "[%s] %s\n", banner, str);
#else
    fprintf(logger_file, "[%s][%s] ", time_str, banner);
#endif
    va_list apcpy;
    va_copy(apcpy, args);
    if (cp_on_file)
    {
        fprintf(cp_on_file, "[%s] ", banner);
        vfprintf(cp_on_file, str, apcpy);
        fprintf(cp_on_file, "\n");
        fflush(cp_on_file);
    }

    vfprintf(logger_file, str, args);
    fprintf(logger_file, "\n");
    fflush(logger_file);
}

void write_logger(const char *banner, FILE *cp_on_file, const char *str, ...)
{
    va_list args;
    va_start(args, str);
    __write_logger(str, banner, cp_on_file, args);
    va_end(args);
}

void write_info(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    __write_logger(msg, "INFO", stdout, args);
    va_end(args);
}

void write_warning(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    __write_logger(msg, "WARNING", stderr, args);
    va_end(args);
}

void write_error(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    __write_logger(msg, "ERROR", stderr, args);
    va_end(args);
}

void raise_panic(int exitCode, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    __write_logger(msg, "PANIC", stderr, args);
    va_end(args);
    exit(exitCode);
}

void logger_close()
{
    fclose(logger_file);
}
