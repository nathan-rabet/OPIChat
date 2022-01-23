#include <criterion/criterion.h>
#include <unistd.h>

#include "logger.h"

Test(logger, logger_init)
{
    logger_init("logger_test_init.log");
    logger_close();

    FILE *fp = fopen("logger_test_init.log", "r");
    cr_assert_not_null(fp);
    fclose(fp);
    unlink("logger_test_init.log");
}

Test(logger, log_info)
{
    logger_init("logger_test_info.log");
    write_info("Hello world");
    logger_close();

    FILE *fp = fopen("logger_test_info.log", "r");
    cr_assert_not_null(fp);

    char buffer[1024] = { 0 };
    fgets(buffer, 1024, fp);
    cr_assert_str_eq(buffer, "[INFO] Hello world\n", "Actual: %s", buffer);

    fclose(fp);
    unlink("logger_test_info.log");
}

Test(logger, log_warning)
{
    logger_init("logger_test_warning.log");
    write_warning("Hello world");
    logger_close();

    FILE *fp = fopen("logger_test_warning.log", "r");
    cr_assert_not_null(fp);

    char buffer[1024] = { 0 };
    fgets(buffer, 1024, fp);
    cr_assert_str_eq(buffer, "[WARNING] Hello world\n", "Actual: %s", buffer);

    fclose(fp);
    unlink("logger_test_warning.log");
}

Test(logger, log_error)
{
    logger_init("logger_test_error.log");
    write_error("Hello world");
    logger_close();

    FILE *fp = fopen("logger_test_error.log", "r");
    cr_assert_not_null(fp);

    char buffer[1024] = { 0 };
    fgets(buffer, 1024, fp);
    cr_assert_str_eq(buffer, "[ERROR] Hello world\n", "Actual: %s", buffer);

    fclose(fp);
    unlink("logger_test_error.log");
}

Test(logger, log_custom)
{
    logger_init("logger_test_custom.log");
    write_logger("CUSTOM", LOG_CP_ON_FILE_NONE, "Hello world");
    logger_close();

    FILE *fp = fopen("logger_test_custom.log", "r");
    cr_assert_not_null(fp);

    char buffer[1024] = { 0 };
    fgets(buffer, 1024, fp);
    cr_assert_str_eq(buffer, "[CUSTOM] Hello world\n", "Actual: %s", buffer);

    fclose(fp);
    unlink("logger_test_custom.log");
}
