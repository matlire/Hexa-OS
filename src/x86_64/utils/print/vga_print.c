#include "vga_print.h"

Char*   terminal_buffer      = (Char*) 0xB8000;
size_t  terminal_col         = 0;
size_t  terminal_row         = 0;
uint8_t terminal_color       = COLOR_WHITE | (COLOR_BLACK << 4);

void terminal_update_cursor (void)
{
    uint16_t pos = (uint16_t)(terminal_row * VGA_WIDTH + terminal_col);

    outb(VGA_CRTC_INDEX, CURSOR_LOCATION_LOW);
    io_wait();
    outb(VGA_CRTC_DATA, pos & 0xFF);
    io_wait();

    outb(VGA_CRTC_INDEX, CURSOR_LOCATION_HIGH);
    io_wait();
    outb(VGA_CRTC_DATA, (pos >> 8) & 0xFF);
    io_wait();
}

void terminal_clear_row(size_t row)
{
    for (size_t i = 0; i < VGA_WIDTH; i++) {
        terminal_buffer[terminal_row * VGA_WIDTH + i] = empty_char();
    }
}

void terminal_clear(void)
{
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
        terminal_clear_row(i);
    }
    terminal_col = 0;
    terminal_row = 0;
    terminal_update_cursor();
}

void terminal_set_color(uint8_t fg_color, uint8_t bg_color)
{
    terminal_color = fg_color | (bg_color << 4);
}

void terminal_write_new_line()
{
    terminal_col = 0;
    terminal_row++;
    if (terminal_row >= VGA_HEIGHT) {
        terminal_row = 0;
    }
    terminal_update_cursor();
}

void terminal_write_char(char c)
{
    if (c == '\n') {
        terminal_write_new_line();
    } else {
        Char curr_char = (Char) 
        {
            character: c,
            color: terminal_color
        };

        terminal_buffer[terminal_row * VGA_WIDTH + terminal_col] = curr_char;
        terminal_col++;
        if (terminal_col >= VGA_WIDTH) {
            terminal_write_new_line();
        }
    }
    terminal_update_cursor();
}

void terminal_write_str(char* str)
{
    while (*str) {
        terminal_write_char(*str);
        str++;
    }
}
