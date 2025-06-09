#include "vga_print.h"

struct Char* buffer = (struct Char*) 0xB8000;
size_t col = 0;
size_t row = 0;
uint8_t color = COLOR_WHITE | (COLOR_BLACK << 4);

void terminal_clear_row(size_t row) {
    struct Char empty = (struct Char) 
    {
        character: ' ',
        color: color
    };

    for (size_t i = 0; i < VGA_WIDTH; i++) {
        buffer[row * VGA_WIDTH + i] = empty;
    }
}

void terminal_clear() {
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
        terminal_clear_row(i);
    }
}

void terminal_set_color(uint8_t fg_color, uint8_t bg_color) {
    color = fg_color | (bg_color << 4);
}

void terminal_write_new_line() {
    col = 0;
    row++;
    if (row >= VGA_HEIGHT) {
        row = 0;
    }
}

void terminal_write_char(char c) {
    if (c == '\n') {
        terminal_write_new_line();
    } else {
        struct Char curr_char = (struct Char) 
        {
            character: c,
            color: color
        };

        buffer[row * VGA_WIDTH + col] = curr_char;
        col++;
        if (col >= VGA_WIDTH) {
            terminal_write_new_line();
        }
    }
}

void terminal_write_str(char* str) {
    while (*str) {
        terminal_write_char(*str);
        str++;
    }
}
