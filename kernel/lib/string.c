#include <cernel/lib/string.h>
#include <cernel/lib/print.h>
#include <cernel/sys/panic.h>
#include <limits.h>
#include <stddef.h>

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

uint64_t strtol(nptr, endptr, base)
	const char * nptr;
	char ** endptr;
	int base;
{
	const char *s;
	unsigned long acc;
	char c;
	unsigned long cutoff;
	int neg, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	s = nptr;
	do {
		c = *s++;
	} while (isspace((unsigned char)c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else {
		neg = 0;
		if (c == '+')
			c = *s++;
	}
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	acc = any = 0;
	if (base < 2 || base > 36)
		goto noconv;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set 'any' if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? (unsigned long)-(LONG_MIN + LONG_MAX) + LONG_MAX
	    : LONG_MAX;
	cutlim = cutoff % base;
	cutoff /= base;
	for ( ; ; c = *s++) {
		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'A' && c <= 'Z')
			c -= 'A' - 10;
		else if (c >= 'a' && c <= 'z')
			c -= 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
        // TODO: after the hack in symbols.c is removed, this can be uncommented and the return type should be set to int64_t
		//acc = neg ? LONG_MIN : LONG_MAX;

	} else if (!any) {
noconv:
        // do something to report error here
        generic_panic("strtol invalid base");
	} else if (neg)
		acc = -(long)acc;
	if (endptr != NULL)
		*endptr = (char *)(any ? s - 1 : nptr);
	return (acc);
}

	
uint8_t isdigit(char c)
{
	return c >= '0' && c <= '9';
}

uint8_t isalpha(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? 1 : 0);
}

uint8_t isspace(char c) 
{
    return (c == '\t' || c == '\n' ||
	    c == '\v' || c == '\f' || c == '\r' || c == ' ' ? 1 : 0);
}