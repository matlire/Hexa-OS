#include "vga_print.h"
#include "isr.h"

void kernel_main() {
    terminal_clear();
    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    terminal_write_str("> Welcome to Hexa OS!\n");

    isr_install();
    __asm__ __volatile__("int $1");

    __asm__ __volatile__("sti");
    while (1) {
        __asm__ __volatile__("hlt");
    }
}