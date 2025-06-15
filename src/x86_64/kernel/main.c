#include "shell.h"
#include "isr.h"

void kernel_main() {
    shell_init();
    isr_install();

    shell_run();
}