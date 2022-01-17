// // Create malloc dynamic library:

// #include <stdio.h>

// #include "xalloc.h"

// /**
//  * @brief My malloc implementation.
//  *
//  */
// #define malloc(sz) \
//     xmalloc(1, sz); \ printf("malloc(%lu)\n", sz);

// /**
//  * @brief My realloc implementation.
//  *
//  */
// #define calloc(nmemb, size) xcalloc(nmemb, size)

// /**
//  * @brief My realloc implementation.
//  *
//  */
// #define realloc(ptr, sz) xrealloc(1, ptr, sz)
