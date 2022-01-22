#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "safe_io.h"

int main(void)
{
    char *buf_out = NULL;
    char buf_in[] = { 'H', 'e', 'l', 'l', 'o', ' ',
                      'W', 'o', 'r', 'l', 'd', '\0' };

    FILE *fp = fopen("safe_io_test.txt", "w+");
    if (fp == NULL)
        return EXIT_FAILURE;

    // Read test
    safe_write(fileno(fp), buf_in, sizeof(buf_in));
    fseek(fp, 0, SEEK_SET);

    int nb_read = safe_read(fileno(fp), (void **)&buf_out);
    printf("Actual number of bytes read: %d\n", nb_read);
}
