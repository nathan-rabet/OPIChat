#include <criterion/criterion.h>
#include <unistd.h>

#include "logger.h"

Test(logger, logger_init)
{
    logger_init("logger_test.log");
    logger_close();

    FILE *fp = fopen("logger_test.log", "r");
    cr_assert_not_null(fp);
    fclose(fp);
    unlink("logger_test.log");
}

Test(logger, log_info)
{
    logger_init("logger_test.log");
    write_info("Hello world");
    logger_close();

    FILE *fp = fopen("logger_test.log", "r");
    cr_assert_not_null(fp);

    char buffer[1024] = { 0 };
    fgets(buffer, 1024, fp);
    cr_assert_str_eq(buffer, "[INFO] Hello world\n", "Actual: %s", buffer);

    fclose(fp);
    unlink("logger_test.log");
}

Test(logger, log_warning)
{
    logger_init("logger_test.log");
    write_warning("Hello world");
    logger_close();

    FILE *fp = fopen("logger_test.log", "r");
    cr_assert_not_null(fp);

    char buffer[1024] = { 0 };
    fgets(buffer, 1024, fp);
    cr_assert_str_eq(buffer, "[WARNING] Hello world\n", "Actual: %s", buffer);

    fclose(fp);
    unlink("logger_test.log");
}

Test(logger, log_error)
{
    logger_init("logger_test.log");
    write_error("Hello world");
    logger_close();

    FILE *fp = fopen("logger_test.log", "r");
    cr_assert_not_null(fp);

    char buffer[1024] = { 0 };
    fgets(buffer, 1024, fp);
    cr_assert_str_eq(buffer, "[ERROR] Hello world\n", "Actual: %s", buffer);

    fclose(fp);
    unlink("logger_test.log");
}

Test(logger, log_custom)
{
    logger_init("logger_test.log");
    write_logger("CUSTOM", LOG_CP_ON_FILE_NONE, "Hello world");
    logger_close();

    FILE *fp = fopen("logger_test.log", "r");
    cr_assert_not_null(fp);

    char buffer[1024] = { 0 };
    fgets(buffer, 1024, fp);
    cr_assert_str_eq(buffer, "[CUSTOM] Hello world\n", "Actual: %s", buffer);

    fclose(fp);
    unlink("logger_test.log");
}
