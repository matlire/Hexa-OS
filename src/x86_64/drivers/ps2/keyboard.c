#include "keyboard.h"

static Keyboard_State_T g_keyboard_state;

void init_keyboard_state (void)
{
    for (int i = 0; i < KEYS_SIZE; i++)
    {
        g_keyboard_state.ascii_states[i] = KEY_UP;
    }

    for (int i = 0; i < EXT_SIZE; i++)
    {
        g_keyboard_state.ext_states[i] = KEY_UP;
    }
    
    g_keyboard_state.waiting_ext    = 0; 
    g_keyboard_state.shift_holded   = 0;
    g_keyboard_state.ctrl_holded    = 0;
    g_keyboard_state.caps_active    = 0;
    g_keyboard_state.num_pad_active = 0;
    q_initialize(&(g_keyboard_state.key_codes_queue));
}

void queue_input (uint8_t keycode)
{
    if (q_isFull(&(g_keyboard_state.key_codes_queue)))
    {
        q_clear(&(g_keyboard_state.key_codes_queue));
    }

    q_enqueue(&(g_keyboard_state.key_codes_queue), keycode);

    return;
}

void terminal_clear_char (size_t col, size_t row)
{
    terminal_get_state()->buffer[row * VGA_WIDTH + col] = empty_char();
    
    return;
}

size_t input_last_row_char (size_t row)
{
    for (size_t i = VGA_WIDTH; i > 0; i--) {
        if (terminal_get_state()->buffer[row * VGA_WIDTH + (i - 1)].character != EMPTY_CHAR_CHAR) {
            return i;
        }
    }

    return 0;
}

void input_clear_buffer (void)
{
    for (uint8_t i = 0; i < INPUT_BUF_SIZE - 1; i++)
    {
        shell_update_input_buffer(i, '\0');
    }

    shell_get_state()->input_ptr = 0;
    shell_set_input_start_row(terminal_get_state()->row);
    shell_set_input_start_col(terminal_get_state()->column);
    
    return;
}


void input_clear_last_char (void)
{
    uint8_t input_ptr = shell_get_state()->input_ptr;

    if (input_ptr == 0) return;
    
    input_ptr -= 1;
    shell_set_input_ptr(input_ptr);
    shell_update_input_buffer(input_ptr, '\0');
    size_t row = terminal_get_state()->row;
    size_t column = terminal_get_state()->column;
    
    if (column == 0)
    {
        if (row == 0)
        {
            input_clear_buffer();
            return;
        }
        row -= 1;
        terminal_set_row(row);
        terminal_set_column(input_last_row_char(row));
    } else {
        column -= 1;
        terminal_set_column(column);
    }

    terminal_clear_char(column, row);
    
    return;
}

void update_input_last_data_row (void)
{
    terminal_set_last_row(shell_get_state()->input_start_row + count(shell_get_state()->input_buffer, '\n') + strlen(shell_get_state()->input_buffer) / VGA_WIDTH);
    return;
}

bool input_check_allowed ()
{
    if (!shell_get_state()->input_allowed || shell_get_state()->input_ptr >= INPUT_BUF_SIZE)
    {
        return 0;
    }

    return 1;
}

bool input_handle_keycode (uint8_t keycode, bool ext)
{
    bool new_state;

    if (keycode >= KEYS_SIZE)
    {
        keycode  -= 128;
        new_state = 0;
    } else {
        new_state = 1;
    }

    if (ext)
    {
        g_keyboard_state.ext_states[keycode] = new_state;
    } else {
        g_keyboard_state.ascii_states[keycode] = new_state;
    }
    
    return new_state;
}

bool input_check_printable (uint8_t keycode, bool holded)
{
    if (!holded || keycode >= KEYS_SIZE) return 0;

    const char *name = keymap[keycode].name;
    for (int i = 0; i < (sizeof(not_printable) / sizeof(*not_printable)); i++)
    {
        if (strcmp(name, not_printable[i]) == 0)
        {
            return 0;
        }
    }

    return 1;
}

void input_redraw (void)
{
    size_t saved_row = terminal_get_state()->row;
    size_t saved_col = terminal_get_state()->column;
    terminal_set_row(shell_get_state()->input_start_row);
    terminal_set_column(shell_get_state()->input_start_col);

    int i = 0;
    while (i < INPUT_BUF_SIZE-1)
    {
        if (shell_get_state()->input_buffer[i] == '\0')
        {
            kputchar(EMPTY_CHAR_CHAR);
            if (terminal_get_state()->row == terminal_get_state()->last_row + 1 || (terminal_get_state()->row == VGA_HEIGHT - 1 && terminal_get_state()->column == VGA_WIDTH - 1))
            {
                break;
            }
        }
        if (shell_get_state()->input_buffer[i] == '\n')
        {
            for (size_t j = terminal_get_state()->column + 1; j < VGA_WIDTH; j++)
            {
                kputchar(EMPTY_CHAR_CHAR);
            }
            terminal_set_column(0);
            kputchar('\n');
        } else if (shell_get_state()->input_buffer[i] != '\0') {
            kputchar(shell_get_state()->input_buffer[i]);
        }
        i++;
    }

    terminal_set_column(saved_col);
    terminal_set_row(saved_row);
    terminal_update_cursor_pos();
}

void input_print_char (uint8_t keycode)
{
    if (keycode >= KEYS_SIZE) return;

    char c;
    if (g_keyboard_state.shift_holded && g_keyboard_state.caps_active) c = keymap[keycode].shifted_caps;
    else if (g_keyboard_state.caps_active)                             c = keymap[keycode].caps;
    else if (g_keyboard_state.shift_holded)                            c = keymap[keycode].shifted;
    else                                                               c = keymap[keycode].normal;

    uint8_t input_ptr = shell_get_state()->input_ptr;
    if (input_ptr == INPUT_BUF_SIZE - 2)
    {
        return;
    }

    if (!shell_get_state()->insert_mode)
    {
        shell_update_input_buffer(input_ptr, c);
        shell_set_input_ptr(input_ptr + 1);
        kputchar(c);
        input_redraw();
    } else {
        int i = INPUT_BUF_SIZE-1;
        while (i > input_ptr && shell_get_state()->input_buffer[input_ptr] != '\0')
        {
            shell_update_input_buffer(i, shell_get_state()->input_buffer[i - 1]);
            i--;
        }
        shell_update_input_buffer(input_ptr, c);
        shell_set_input_ptr(input_ptr + 1);
        i = 0;
        size_t saved_row = terminal_get_state()->row;
        size_t saved_col = terminal_get_state()->column;
        terminal_set_column(shell_get_state()->input_start_col);
        terminal_set_row(shell_get_state()->input_start_row);
        while (i < strlen(shell_get_state()->input_buffer))
        {
            kputchar(shell_get_state()->input_buffer[i++]);
        }
        if (c != '\n')
        {
            terminal_set_row(saved_row);
            terminal_set_column(saved_col + 1);
        } else {
            terminal_set_column(0);
            input_redraw();
        }
    }
    
    update_input_last_data_row();
    terminal_update_cursor_pos();

    return;
}


Keyboard_State_T* keyboard_get_state (void)
{
    return &g_keyboard_state;
}

void keyboard_set_waiting_ext    (bool val)
{
    g_keyboard_state.waiting_ext = val;
}

void keyboard_set_shift_holded   (bool val)
{
    g_keyboard_state.shift_holded = val;
}

void keyboard_set_ctrl_holded    (bool val)
{
    g_keyboard_state.ctrl_holded = val;
}

void keyboard_set_caps_active    (bool val)
{
    g_keyboard_state.caps_active = val;
}

void keyboard_set_num_pad_active (bool val)
{
    g_keyboard_state.num_pad_active = val;
}
