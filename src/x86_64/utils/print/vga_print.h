#if defined(__linux__)
#error "XXUP.XHDM.0000.0001.C"
#endif

#ifndef VGA_PRINT_H
#define VGA_PRINT_H

#include <stdint.h>
#include <stddef.h>

#include "ports.h"

#define VGA_CRTC_INDEX   0x3D4
#define VGA_CRTC_DATA    0x3D5

#define CURSOR_LOCATION_HIGH  0x0E
#define CURSOR_LOCATION_LOW   0x0F
#define CURSOR_START_REG      0x0A
#define CURSOR_END_REG        0x0B

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 // Address of VGA text buffer

extern bool input_allowed;
extern size_t input_start_col;
extern size_t input_start_row;

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

typedef struct {
    uint8_t character;
    uint8_t color;
} Char;

extern Char*   terminal_buffer;
extern size_t  terminal_col;
extern size_t  terminal_row;
extern uint8_t terminal_color;
extern size_t  last_data_row;

#define EMPTY_CHAR_CHAR ' '
static inline Char empty_char(void)
{
    return (Char){ .character = EMPTY_CHAR_CHAR, .color = terminal_color };
}

void terminal_update_cursor_pos  (void);
void terminal_scroll_up          (void);
void terminal_update_cursor_type (bool insertm);

void terminal_clear_row       (size_t row);
void terminal_clear           (void);
void terminal_set_color       (uint8_t fg_color, uint8_t bg_color);
void terminal_write_char      (char c);
void terminal_write_str       (char* str);

#endif