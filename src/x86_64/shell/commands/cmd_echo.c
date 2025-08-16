#include "commands.h"
#include "vga_print.h"
#include "strings.h"

static int cmd_echo (int argc, Command_Token_T **argv)
{
    bool help  = 0;
    char *text = NULL;

    for (int i = 1; i < argc; i++)
    {
        Command_Token_T *token = argv[i];

        if (token->type == TOKEN_BOOL_FLAG && strcmp(token->name, "help") == 0) help = 1;
        if (token->type == TOKEN_TEXT_FLAG && strcmp(token->name, "text") == 0) text = token->value;
    }

    if (help)
    {
        terminal_write_str("Usage: echo [--text=\"...\"] [args...]\n");
        terminal_write_str("  --help        Show this help message\n");
        terminal_write_str("  --text=\"...\"  Print the given text"); 
        return 0;
    }

    if (text)
    {
        terminal_write_str(text);
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        Command_Token_T *token = argv[i];
        if (token->type != TOKEN_NORMAL) continue;
        terminal_write_str(token->value);
        terminal_write_str(" ");
    }
    
    return 0;
}

SHELL_COMMAND("echo", "Echo given input.", cmd_echo);

