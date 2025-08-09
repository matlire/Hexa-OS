#ifndef CHARS_H
#define CHARS_H

#include <stdint.h>

#define KEYS_SIZE 128
#define EXT_SIZE  128
#define CTRL_SIZE 32
#define NPRT_SIZE 64

typedef struct {
    const char* name;
    char normal;
    char shifted;
    char caps;
    char shifted_caps;
} Key_Map_Entry;

extern const Key_Map_Entry keymap[KEYS_SIZE];

typedef struct {
    const char* name;
    char normal;
    char numlock;
} Ext_Key_Map_Entry;

extern const Ext_Key_Map_Entry ext_keymap[EXT_SIZE];

extern const char* not_printable[NPRT_SIZE];
extern const char  ctrl_break[CTRL_SIZE];

typedef enum {
    KEY_NUMLOCK     = 0x45,
    KEY_ENTER       = 0x1C,
    KEY_BACKSPACE   = 0x0E,
    KEY_CAPSLOCK    = 0x3A,
    KEY_LSHIFT      = 0x2A,
    KEY_RSHIFT      = 0x36,
    KEY_LCTRL       = 0x1D,
    KEY_RCTRL       = 0x1D,
    KEY_KP0         = 0x52,
    KEY_ARROW_LEFT  = 0x4B,
    KEY_ARROW_RIGHT = 0x4D,
    KEY_ARROW_UP    = 0x48,
    KEY_ARROW_DOWN  = 0x50,
    KEY_DELETE      = 0x53,
    KEY_KP7         = 0x47,
    KEY_KP1         = 0x4F,
} Major_Codes;

#endif