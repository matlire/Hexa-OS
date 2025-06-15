#include "strings.h"

unsigned int strlen (char *str)
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
		char buf = str[start];
		str[start] = str[end];
		str[end] = buf;
		start++;
		end--;
	}
}

bool strcmp (char *str1, char *str2)
{
    while (*str1 && *str2) {
        if (*str1 != *str2) return 0;
        str1++;
        str2++;
    }

    return (*str1 == '\0' && *str2 == '\0');
}

void int2ascii (int n, char *str)
{
	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return;
	}

	bool is_neg = 0;
	if (n < 0)
	{
		is_neg = 1;
		n = -n;
	}

	unsigned int i = 0;
	while (n > 0)
	{
		str[i++] = '0' + (n % 10);
		n /= 10;
	}

	if (is_neg)
	{
		str[i++] = '-';
	}

	str[i] = '\0';
	reverse(str);
	return;
}

void hex2ascii(int n, char *str) {
    const char *hex_chars = "0123456789ABCDEF";
    int i = 0;

    if (n == 0) {
        str[i++] = '0';
        str[i++] = 'x';
        str[i++] = '0';
        str[i] = '\0';
        reverse(str);
        return;
    }

    while (n != 0) {
        str[i++] = hex_chars[n % 16];
        n /= 16;
    }

    str[i++] = 'x';
    str[i++] = '0';
    str[i] = '\0';

    reverse(str);
    return;
}
