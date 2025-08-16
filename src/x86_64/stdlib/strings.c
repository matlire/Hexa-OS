#include "strings.h"

uint16_t strlen (const char *str)
{
	unsigned int len = 0;
	while (str[len] != '\0') { len++; }
	return len;
}

void reverse (char *str)
{
	unsigned int start = 0;
	unsigned int end   = strlen(str);
	if (end == start) { return; }
	end--;

	while (start < end)
	{
		char buf   = str[start];
		str[start] = str[end];
		str[end]   = buf;
		start++;
		end--;
	}
}

int strcmp (const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

int strncmp (const char *str1, const char *str2, short int n)
{
    while (n > 0 && *str1 && (*str1 == *str2)) {
        str1++;
        str2++;
        n--;
    }

    if (n == 0) {
        return 0;
    }

    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

uint16_t count (const char *str, const char chr)
{
	uint16_t res = 0;
	while (*str)
	{
		if (*str == chr)
		{
			res++;
		}
		str++;
	}

	return res;
}

char *strchr (const char *str, int c)
{
    while (*str)
    {
        if (*str == (char)c)
        {
            return (char *)str;
        }
        str++;
    }

    if (c == '\0')
    {
        return (char *) str;
    }

    return 0;
}

void int2ascii (const int n, char *str)
{
	int ln = n;
	
	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return;
	}

	bool is_neg = 0;
	if (ln < 0)
	{
		is_neg = 1;
		ln = -ln;
	}

	unsigned int i = 0;
	while (ln > 0)
	{
		str[i++] = '0' + (ln % 10);
		ln /= 10;
	}

	if (is_neg)
	{
		str[i++] = '-';
	}

	str[i] = '\0';
	reverse(str);
	return;
}

void hex2ascii(const int n, char *str) {
    const char *hex_chars = "0123456789ABCDEF";
    int i = 0;
    int ln = 0;

    if (ln == 0) {
        str[i++] = '0';
        str[i++] = 'x';
        str[i++] = '0';
        str[i] = '\0';
        reverse(str);
        return;
    }

    while (ln != 0) {
        str[i++] = hex_chars[ln % 16];
        ln /= 16;
    }

    str[i++] = 'x';
    str[i++] = '0';
    str[i] = '\0';

    reverse(str);
    return;
}
