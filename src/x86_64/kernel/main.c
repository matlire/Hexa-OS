#include "shell.h"
#include "isr.h"
#include "keyboard.h"

void kernel_main() {
    init_terminal();
    shell_init();
    isr_install();
    init_keyboard_state();

    shell_run();
}