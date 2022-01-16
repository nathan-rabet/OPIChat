#include "xalloc.h"

#include <err.h>
#include <stdlib.h>

void *xmalloc(size_t nmemb, size_t size)
{
    size_t total_size = 0;
    if (__builtin_umull_overflow(nmemb, size, &total_size))
        errx(1, "xmalloc: overflow detected : %zu * %zu", nmemb, size);
    void *res = malloc(total_size);
    if (!res)
        err(1, "xmalloc: malloc failed, size: %zu", total_size);

    return res;
}

void *xcalloc(size_t nmemb, size_t size)
{
#ifdef DEBUG // Disabled in production for performance reasons
    if (__builtin_umull_overflow(nmemb, size, NULL))
        errx(1, "xmalloc: overflow detected : %zu * %zu", nmemb, size);
#endif
    void *res = calloc(nmemb, size);
    if (!res)
        err(1, "xcalloc: calloc failed, nmemb: %zu, size: %zu", nmemb, size);

    return res;
}

void *xrealloc(void *ptr, size_t nmemb, size_t size)
{
    size_t total_size = 0;
    if (__builtin_umull_overflow(nmemb, total_size, &total_size))
        errx(1, "xrealoc: overflow detected : %zu * %zu", nmemb, size);
    void *res = realloc(ptr, size);
    if (!res)
        err(1, "xrealloc: realloc failed, size: %zu", total_size);

    return res;
}
