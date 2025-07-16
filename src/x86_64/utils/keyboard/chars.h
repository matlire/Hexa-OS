#ifndef CHARS_H
#define CHARS_H

#include <stdint.h>

#define NPRT_SIZE  64
#define CTRL_SIZE  32
#define ASCII_SIZE 128
#define EXT_SIZE   16
#define KEYS_SIZE  128

extern const char *not_printable[NPRT_SIZE];
extern const char  ctrl_break[CTRL_SIZE];

extern const char *ascii_names[ASCII_SIZE];
extern const char  ascii_normal[ASCII_SIZE];
extern const char  ascii_shifted[ASCII_SIZE];
extern const char  ascii_caps[ASCII_SIZE];
extern const char  ascii_shifted_caps[ASCII_SIZE];

extern const char *ext_names[EXT_SIZE];
extern const char  ext_normal[EXT_SIZE];
extern const char  ext_numed[EXT_SIZE];

#endif