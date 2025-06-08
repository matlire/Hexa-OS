#include "vga_print.h"

void clear_row(size_t row) {
    struct Char empty = (struct Char) 
    {
        character: ' ',
        color: color
    };

    for (size_t i = 0; i < NUM_COLS; i++) {
        buffer[row * NUM_COLS + i] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
}

void print_set_color(uint8_t fg_color, uint8_t bg_color) {
    color = fg_color | (bg_color << 4);
}

void print_new_line() {
    col = 0;
    row++;
    if (row >= NUM_ROWS) {
        row = 0;
    }
}

void print_char(char c) {
    if (c == '\n') {
        print_new_line();
    } else {
        struct Char curr_char = (struct Char) 
        {
            character: c,
            color: color
        };

        buffer[row * NUM_COLS + col] = curr_char;
        col++;
        if (col >= NUM_COLS) {
            print_new_line();
        }
    }
}

void print_str(char* str) {
    while (*str) {
        print_char(*str);
        str++;
    }
}
