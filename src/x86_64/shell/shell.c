#include "shell.h"

static Shell_State_T g_shell_state;

void shell_init (void)
{
    g_shell_state.input_allowed   = 0;
    g_shell_state.insert_mode     = 1;
    g_shell_state.input_start_col = 0;
    g_shell_state.input_start_row = 0;
    g_shell_state.input_ptr       = 0;

    for (int i = 0; i < INPUT_BUF_SIZE; i++)
    {
        g_shell_state.input_buffer[i] = '\0';
    }

	terminal_clear();
    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    kprint("> Welcome to Hexa OS!\n");
    kprint("> Write help to get started\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    kprint("\n");
    kprint("> ");
    terminal_update_cursor_type(g_shell_state.insert_mode);
    terminal_update_cursor_pos();

    g_shell_state.input_allowed   = 1;
    g_shell_state.input_start_col = terminal_get_state()->column;
    g_shell_state.input_start_row = terminal_get_state()->row;
    
    return;
}

void shell_run (void)
{
	__asm__ __volatile__("sti");
    while (1) {
        __asm__ __volatile__("hlt");
        shell_handle_input();
    }
}

void shell_execute (char *input)
{
    g_shell_state.input_allowed = 0;

    terminal_set_row(terminal_get_state()->last_row);
	kprint("\n");
	
    Command_Token_T *tokens[MAX_TOKENS];
    int argc = commands_tokenize(input, (Command_Token_T **)tokens);
    bool found = 0;
    if (argc != 0)
    {
        const Command_Function_T *cmd = __start_shell_commands;
        while (cmd < __stop_shell_commands && !found)
        {
            if (strcmp(tokens[0]->value, cmd->name) == 0)
            {
                cmd->func(argc, tokens);
                found = 1;
                break;
            }
            cmd++;
        }
        if (!found)
        {
            kprint("Command not found.");
        }
    } 

	kprint("\n");
    kprint("> ");

    g_shell_state.input_allowed   = 1;
    g_shell_state.input_start_col = terminal_get_state()->column;
    g_shell_state.input_start_row = terminal_get_state()->row;
    terminal_update_cursor_pos();

	return;
}

void shell_handle_input (void)
{
    while (!q_isEmpty(&(keyboard_get_state()->key_codes_queue)))
    {
        int keycode = q_dequeue(&(keyboard_get_state()->key_codes_queue));
        if (!input_check_allowed()) { return; }

        if (keycode == 0xE0)
        {
            keyboard_set_waiting_ext(1);
            return;
        }
        if (keyboard_get_state()->waiting_ext)
        {
            bool holded = input_handle_keycode(keycode, 1);
            if (holded == 0) { keycode -= 128; }

            input_ext_check_arrows(keycode, holded);
            check_ext_del(keycode, holded);
            keyboard_set_waiting_ext(0);
            return;
        }

        if (keycode > 2 * KEYS_SIZE || keycode <= 0)
        {
            return;
        }

        bool holded = input_handle_keycode(keycode, 0);
        if (holded == 0) { keycode -= 128; }

        if (input_check_enter(keycode, holded)) { return; }

        if (input_check_num(keycode, holded)) { return; }

        if (input_check_insert(keycode, holded)) { return;}

        if (input_check_backspace(keycode, holded)) { return; }

        if (input_check_caps(keycode, holded)) { return; }

        if (input_check_shift_ctrl(keycode, holded)) { return; }

        if (input_check_hend(keycode, holded)) { return; }

        if (input_check_printable(keycode, holded))
        {
            input_print_char(keycode);
        }
    }
    return;
}

Shell_State_T* shell_get_state (void)
{
    return &g_shell_state;
}

void shell_update_input_buffer (uint8_t pos, char chr)
{
    g_shell_state.input_buffer[pos] = chr;
}

void shell_set_input_ptr (uint8_t val)
{
    g_shell_state.input_ptr = val;
}

void shell_set_input_allowed (bool val)
{
    g_shell_state.input_allowed = val;
}

void shell_set_insert_mode (bool val)
{
    g_shell_state.insert_mode = val;
}

void shell_set_input_start_col (size_t val)
{
    g_shell_state.input_start_col = val;
}

void shell_set_input_start_row (size_t val)
{
    g_shell_state.input_start_row = val;
}
