#pragma once

#include <stdint.h>
#include <stddef.h>

#if defined(__linux__)
#error "XXUP.XHDM.0000.0001.C"
#endif

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 // Address of VGA text buffer

enum {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GRAY = 7,
    COLOR_DARK_GRAY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_YELLOW = 14,
    COLOR_WHITE = 15
};

struct Char {
    uint8_t character;
    uint8_t color;
};

extern struct Char* terminal_buffer;

extern size_t       terminal_col;
extern size_t       terminal_row;
extern uint8_t      terminal_color;

void terminal_clear();
void terminal_set_color(uint8_t fg_color, uint8_t bg_color);
void terminal_write_char(char c);
void terminal_write_str(char* str);