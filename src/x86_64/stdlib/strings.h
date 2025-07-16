#ifndef STRINGS_H
#define STRINGS_H

#include <stdint.h>

uint16_t strlen  (const char *str);
void reverse 	 (char *str);
short int strcmp (const char *str1, const char *str2);
uint16_t count   (const char *str, const char chr);

void int2ascii (const int n, char *str);
void hex2ascii (const int n, char *str);

#endif