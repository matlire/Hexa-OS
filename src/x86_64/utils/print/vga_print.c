#include "vga_print.h"

Char* buffer        = (Char*) 0xB8000;
size_t col          = 0;
size_t row          = 0;
uint8_t color       = COLOR_WHITE | (COLOR_BLACK << 4);

void terminal_update_cursor (void)
{
    uint16_t pos = (uint16_t)(row * VGA_WIDTH + col);

    outb(VGA_CRTC_INDEX, CURSOR_LOCATION_LOW);
    io_wait();
    outb(VGA_CRTC_DATA, pos & 0xFF);
    io_wait();

    // Now set the High byte
    outb(VGA_CRTC_INDEX, CURSOR_LOCATION_HIGH);
    io_wait();
    outb(VGA_CRTC_DATA, (pos >> 8) & 0xFF);
    io_wait();
}

void int_to_ascii (int value, char *buffer)
{
    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    bool negative = 0;
    if (value < 0)
    {
        negative = 1;
        value = -value;
    }

    int i = 0;
    const int chr_len = 12;
    char temp[chr_len];
    while (value > 0 && i < chr_len)
    {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (negative)
    {
        temp[i++] = '-';
    }

    int j = 0;
    while (i > 0 && j < chr_len)
    {
        buffer[j++] = temp[--i];
    }

    buffer[j] = '\0';
    return;
}

void terminal_clear_last_char (void)
{
    Char empty = (Char) 
    {
        character: ' ',
        color: color
    };

    if (col == 0)
    {
        if (row == 0)
        {
            row = VGA_HEIGHT;
        }
        row -= 1;
        col = VGA_WIDTH-1;
    } else {
        col -= 1;
    }
    buffer[row * VGA_WIDTH + col] = empty;
    terminal_update_cursor();
}

void terminal_clear_row(size_t row)
{
    Char empty = (Char) 
    {
        character: ' ',
        color: color
    };

    for (size_t i = 0; i < VGA_WIDTH; i++) {
        buffer[row * VGA_WIDTH + i] = empty;
    }
}

void terminal_clear(void)
{
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
        terminal_clear_row(i);
    }
    col = 0;
    row = 0;
    terminal_update_cursor();
}

void terminal_set_color(uint8_t fg_color, uint8_t bg_color)
{
    color = fg_color | (bg_color << 4);
}

void terminal_write_new_line()
{
    col = 0;
    row++;
    if (row >= VGA_HEIGHT) {
        row = 0;
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
            color: color
        };

        buffer[row * VGA_WIDTH + col] = curr_char;
        col++;
        if (col >= VGA_WIDTH) {
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
