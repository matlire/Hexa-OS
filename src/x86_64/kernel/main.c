#include "vga_print.h"

void kernel_main() {
    terminal_clear();
    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    terminal_write_str("> Welcome to Hexa OS!");
}