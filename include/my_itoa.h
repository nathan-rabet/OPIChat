#ifndef MY_ITOA_H
#define MY_ITOA_H

#include <stdbool.h>

/**
 * @brief Reverses a string
 *
 * @param str
 * @param len
 */
void reverse(char str[], int len);

/**
 * @brief Swaps two char*
 *
 * @param a
 * @param b
 */
void swap(char *a, char *b);

/**
 * @brief Converts an int into a string
 *
 * @param value
 * @param s
 * @return char*
 */
char *my_itoa(int value, char *s);

#endif /* MY_ITOA_H */
