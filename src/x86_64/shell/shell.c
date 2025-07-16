#include "shell.h"

bool input_allowed = 0;
bool insertm 	   = 1;

size_t input_start_col = 0;
size_t input_start_row = 0;

void shell_init (void)
{
	terminal_clear();
    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    terminal_write_str("> Welcome to Hexa OS!\n");
    terminal_write_str("> Write help to get started\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_write_str("\n");
    terminal_write_str("> ");
    terminal_update_cursor_type(insertm);
    terminal_update_cursor_pos();
    input_allowed = 1;
    input_start_col = terminal_col;
    input_start_row = terminal_row;
    return;
}

void shell_run (void)
{
	__asm__ __volatile__("sti");
    while (1) {
        __asm__ __volatile__("hlt");
    }
}

void shell_execute (char *input)
{
    input_allowed = 0;
    terminal_row = last_data_row;
	terminal_write_str("\n");
	terminal_write_str(input);
	terminal_write_str("\n");
    terminal_write_str("> ");
    input_allowed = 1;
    input_start_col = terminal_col;
    input_start_row = terminal_row;
    terminal_update_cursor_pos();
	return;
}
