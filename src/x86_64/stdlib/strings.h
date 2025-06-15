#ifndef STRINGS_H
#define STRINGS_H

unsigned int strlen  (char *str);
void reverse (char *str);
bool strcmp  (char *str1, char *str2);

void int2ascii (int n, char *str);
void hex2ascii (int n, char *str);

#endif