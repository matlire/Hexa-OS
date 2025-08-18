#include "shell.h"
#include "isr.h"
#include "keyboard.h"
#include "time_core.h"

void kernel_main() {
    init_terminal();
    shell_init();
    isr_install();
    time_core_init_auto();
    init_keyboard_state();

    shell_run();
}
