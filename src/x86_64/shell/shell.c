#include "shell.h"

bool input_allowed = 0;

void shell_init (void)
{
	terminal_clear();
    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    terminal_write_str("> Welcome to Hexa OS!\n");
    terminal_write_str("> Write help to get started\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_write_str("\n");
    terminal_write_str("> ");
    input_allowed = 1;
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
	terminal_write_str("\n");
    terminal_write_str("> ");
	return;
}
