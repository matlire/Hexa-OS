#include "commands.h"
#include "vga_print.h"

static int cmd_info (int argc, Command_Token_T **argv)
{
    kprint("System Info:\n");
    kprint("  Arch: x86-64\n");
    kprint("  Kernel: Hexa OS kernel");
    return 0;
}

SHELL_COMMAND("info", "Show system information", cmd_info);
