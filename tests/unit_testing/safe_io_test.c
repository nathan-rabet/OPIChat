#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "safe_io.h"

Test(safe_io, safe_write, .init = cr_redirect_stdout)
{
    char buf_in[1024] = { 'H', 'e', 'l', 'l', 'o', ' ',
                          'W', 'o', 'r', 'l', 'd', '\0' };

    // Write test
    int isOk = safe_write(STDOUT_FILENO, buf_in, strlen(buf_in)) == 0;
    cr_assert(isOk, "safe_write failed");
    cr_stdout_match_str("Hello World");
}

Test(safe_io, safe_read)
{
    char *buf_out = NULL;
    char buf_in[] = { 'H', 'e', 'l', 'l', 'o', ' ',
                      'W', 'o', 'r', 'l', 'd', '\0' };

    FILE *fp = fopen("safe_io_test.txt", "w+");
    cr_assert(fp != NULL, "safe_read failed");

    // Read test
    safe_write(fileno(fp), buf_in, sizeof(buf_in));
    fseek(fp, 0, SEEK_SET); // Set to the beginning of the file for reading

    int nb_read = safe_read(fileno(fp), (void **)&buf_out);
    cr_expect_eq(nb_read, sizeof(buf_in), "Actual number of bytes read: %d",
                 nb_read);
    cr_expect_str_eq(buf_out, buf_in);

    free(buf_out);
    fclose(fp);
    unlink("safe_io_test.txt");
}
