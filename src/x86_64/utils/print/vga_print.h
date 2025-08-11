#if defined(__linux__)
#error "XXUP.XHDM.0000.0001.C"
#endif

#ifndef VGA_PRINT_H
#define VGA_PRINT_H

#include <stdint.h>
#include <stddef.h>

#include "ports.h"
#include "shell.h"

typedef enum {
    VGA_CRTC_INDEX = 0x3D4,
    VGA_CRTC_DATA  = 0x3D5,

    CURSOR_LOCATION_HIGH = 0x0E,
    CURSOR_LOCATION_LOW  = 0x0F,
    CURSOR_START_REG     = 0x0A,
    CURSOR_END_REG       = 0x0B,
} VGA_Driver_Consts;

typedef enum {
    VGA_WIDTH  = 80,
    VGA_HEIGHT = 25,
    VGA_MEMORY = 0xB8000, // Address of VGA text buffer
} VGA_Props;

typedef enum {
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
} Colors;

typedef struct {
    uint8_t character;
    uint8_t color;
} Char;

typedef struct {
    Char*   buffer;
    size_t  column;
    size_t  row;
    uint8_t color;
    size_t  last_row;
} Terminal;

#define EMPTY_CHAR_CHAR ' '

void init_terminal (void);

Terminal* terminal_get_state  (void);

void terminal_set_row      (size_t row);
void terminal_set_column   (size_t column);
void terminal_set_last_row (size_t last_row);

Char empty_char (void);

void terminal_update_cursor_pos  (void);
void terminal_scroll_up          (void);
void terminal_update_cursor_type (bool insertm);

void terminal_clear_row       (size_t row);
void terminal_clear           (void);
void terminal_set_color       (uint8_t fg_color, uint8_t bg_color);
void terminal_write_char      (char c);
void terminal_write_str       (char* str);

#endif