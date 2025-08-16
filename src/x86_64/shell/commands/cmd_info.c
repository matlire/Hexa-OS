#include "commands.h"
#include "vga_print.h"

static int cmd_info (int argc, Command_Token_T **argv)
{
    terminal_write_str("System Info:\n");
    terminal_write_str("  Arch: x86-64\n");
    terminal_write_str("  Kernel: Hexa OS kernel");
    return 0;
}

SHELL_COMMAND("info", "Show system information", cmd_info);
