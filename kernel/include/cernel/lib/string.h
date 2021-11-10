#pragma once

#include <stdint.h>

/**
 *  @brief calculates the length of a string
 *  @param str pointer to a string
 *  @return length of the specified string
 */
uint64_t strlen(char *str);

int64_t strcmp(char *s1, char *s2);

uint64_t strtol(const char *nptr, char **endptr, int base);