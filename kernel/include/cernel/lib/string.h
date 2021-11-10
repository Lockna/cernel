#pragma once

#include <stdint.h>

/**
 *  @brief calculates the length of a string
 *  @param str pointer to a string
 *  @return length of the specified string
 */
uint64_t strlen(char *str);

int64_t strcmp(char *s1, char *s2);

int64_t strtol(const char *nptr, char **endptr, int base);

/**
 *  @brief checks whether a char is a digit
 *  @param ch the char to check
 *  @return 1 if the char is digit, otherwise 0
 */
uint8_t isdigit(char c);

uint8_t isalpha(char c);

uint8_t isspace(char c);