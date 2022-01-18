#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

bool check_is_number(const char *s)
{
    if (s == NULL)
        return 0;

    while (*s)
    {
        if (!isdigit(*s))
            return 0;
        s++;
    }
    return 1;
}
