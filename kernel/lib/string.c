#include <cernel/lib/string.h>
#include <cernel/lib/print.h>

uint64_t strlen(char *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i++;
    }

    return i;
}

int64_t strcmp(char *s1, char *s2)
{
    int64_t len1 = strlen(s1);
    int64_t len2 = strlen(s2);

    int64_t diff = len2 - len1;

    if (diff != 0)
        return diff;
    
    for (int i = 0; i < len1; i++) {
        if (s1[i] != s2[i])
            return s2[i] - s1[i];
    }

    return 0;
}

uint64_t strtol(const char *nptr, char **endptr, int base)
{
    // ignore wrong values for base
    if (base < 2 || base > 36) {
        return 0;
    }    
}