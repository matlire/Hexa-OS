#include "vga_print.h"

static Terminal_T g_terminal;

void init_terminal (void)
{
    g_terminal.buffer   = (Char_T*) 0xB8000;
    g_terminal.column   = 0;
    g_terminal.row      = 0;
    g_terminal.color    = COLOR_WHITE | (COLOR_BLACK << 4);
    g_terminal.last_row = 0;
}

Terminal_T* terminal_get_state (void) {
    return &g_terminal;
}

void terminal_set_row (size_t row)
{
    g_terminal.row = row;
}

void terminal_set_column (size_t column)
{
    g_terminal.column = column;
}

void terminal_set_last_row (size_t last_row)
{
    g_terminal.last_row = last_row;
}

Char_T empty_char(void)
{
    return (Char_T){ .character = EMPTY_CHAR_CHAR, .color = g_terminal.color };
}

void terminal_update_cursor_pos (void)
{
    uint16_t pos = (uint16_t) (g_terminal.row * VGA_WIDTH + g_terminal.column);

    outb(VGA_CRTC_INDEX, CURSOR_LOCATION_LOW);
    io_wait();
    outb(VGA_CRTC_DATA, pos & 0xFF);
    io_wait();

    outb(VGA_CRTC_INDEX, CURSOR_LOCATION_HIGH);
    io_wait();
    outb(VGA_CRTC_DATA, (pos >> 8) & 0xFF);
    io_wait();
}

void terminal_update_cursor_type (bool insertm)
{
    uint8_t start = insertm ? 0x00 : 0x0E;
    uint8_t end   = insertm ? 0x0D : 0x0F;

    outb(VGA_CRTC_INDEX, CURSOR_START_REG);
    io_wait();
    outb(VGA_CRTC_DATA, start);
    io_wait();

    outb(VGA_CRTC_INDEX, CURSOR_END_REG);
    io_wait();
    outb(VGA_CRTC_DATA, end);
    io_wait();
}

void terminal_clear_row (size_t row)
{
    for (size_t i = 0; i < VGA_WIDTH; i++) {
        g_terminal.buffer[g_terminal.row * VGA_WIDTH + i] = empty_char();
    }
}

void terminal_clear (void)
{
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
        terminal_clear_row(i);
    }
    g_terminal.column   = 0;
    g_terminal.row      = 0;
    g_terminal.last_row = 0;
    terminal_update_cursor_pos();
}

void terminal_set_color (uint8_t fg_color, uint8_t bg_color)
{
    g_terminal.color = fg_color | (bg_color << 4);
}

void terminal_scroll_up (void)
{
    for (size_t row = 1; row < VGA_HEIGHT; row++) {
        for (size_t col = 0; col < VGA_WIDTH; col++) {
            g_terminal.buffer[(row - 1) * VGA_WIDTH + col] = g_terminal.buffer[row * VGA_WIDTH + col];
        }
    }

    for (size_t col = 0; col < VGA_WIDTH; col++) {
        g_terminal.buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = empty_char();
    }

    if (g_terminal.row > 0)    { g_terminal.row--; }
    if (shell_get_state()->input_start_row > 0) { shell_set_input_start_row(shell_get_state()->input_start_row - 1);; }
}

void terminal_write_new_line (void)
{
    g_terminal.column   = 0;
    g_terminal.row     += 1;

    if (g_terminal.row >= VGA_HEIGHT) {
        terminal_scroll_up();
        g_terminal.row       = VGA_HEIGHT - 1;
        g_terminal.last_row -= 1;
    }
    
    g_terminal.last_row     += 1;
}

void kputchar (char c)
{
    if (c == '\n') {
        terminal_write_new_line();
    } else {
        if (c == '\0')
        {
            c = EMPTY_CHAR_CHAR;
        }
        Char_T curr_char = (Char_T) 
        {
            character: c,
            color: g_terminal.color
        };

        g_terminal.buffer[g_terminal.row * VGA_WIDTH + g_terminal.column] = curr_char;
        g_terminal.column += 1;

        if (g_terminal.column >= VGA_WIDTH) {
            terminal_write_new_line();
        }
    }
}

void kprint (char* str)
{
    while (*str) {
        kputchar(*str);
        str++;
    }
}
