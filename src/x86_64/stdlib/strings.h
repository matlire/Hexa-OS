#ifndef STRINGS_H
#define STRINGS_H

#include <stdint.h>

uint16_t strlen (const char *str);
void reverse    (char *str);
int strcmp      (const char *str1, const char *str2);
int strncmp     (const char *str1, const char *str2, short int n);
uint16_t count  (const char *str, const char chr);
char *strchr (const char *str, int c);

void int2ascii (const int n, char *str);
void hex2ascii (const int n, char *str);

#endif
