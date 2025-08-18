#include "commands.h"
#include "vga_print.h"

static int cmd_info (int argc, Command_Token_T **argv)
{
    kprint("Commands:\n");
    kprint("  help: Show built in commands\n");
    kprint("  info: Get system info\n");
    kprint("  echo: Print given text");
    return 0;
}

SHELL_COMMAND("help", "Show built in commands", cmd_info);
