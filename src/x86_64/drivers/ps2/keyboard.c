#include "keyboard.h"

void handle_input (uint8_t keycode)
{
    if (input_check_allowed == 0) { return; }

    if (keycode == 0xe0)
    {
        ext = 1;
        return;
    }
    if (ext)
    {
        ext = 0;
        return;
    }
    if (keycode > ASCII_SIZE && keycode < 128)
    {
        return;
    }

    bool holded = input_handle_keycode(keycode);
    if (holded == 0) { keycode -= 128; }

    if (input_check_enter(keycode, holded)) { return; }

    if (input_check_backspace(keycode, holded)) { return; }

    if (input_check_caps(keycode, holded)) { return; }

    if (input_check_shifting(keycode, holded)) { return; }

    if (input_check_printable(keycode, holded))
    {
        input_print_char(keycode);
    }

    return;
}

void terminal_clear_char (size_t col, size_t row)
{
    terminal_buffer[row * VGA_WIDTH + col] = empty_char();
    return;
}

size_t input_last_row_char(size_t row)
{
    for (size_t i = VGA_WIDTH; i > 0; i--) {
        if (terminal_buffer[row * VGA_WIDTH + (i-1)].character != EMPTY_CHAR_CHAR) {
            return i;
        }
    }

    return 0;
}

void input_clear_buffer (void)
{
    for (uint8_t i = 0; i < INPUT_BUF_SIZE; i++)
    {
        input_buffer[i] = '\0';
    }
    input_ptr = 0;
    return;
}

void input_clear_last_char (void)
{
    input_buffer[input_ptr] = '\0';
    if (terminal_col == 0)
    {
        if (terminal_row == 0)
        {
            input_clear_buffer();
            return;
        }
        terminal_row--;
        terminal_col = input_last_row_char(terminal_row);
    } else {
        terminal_col--;
    }
    terminal_clear_char(terminal_col, terminal_row);
    input_ptr--;
    return;
}

bool input_check_allowed ()
{
    if (!input_allowed || input_ptr >= INPUT_BUF_SIZE || input_ptr < 0)
    {
        return 0;
    }
    return 1;
}

bool input_handle_keycode (uint8_t keycode)
{
    bool new_state;
    if (keycode > ASCII_SIZE && ext == 0)
    {
        keycode -= 128;
        new_state = 0;
        return 0;
    } else {
        new_state = 1;
    }
    ascii_states[keycode] = new_state;
    return new_state;
}

bool input_check_enter (uint8_t keycode, bool holded)
{
    if (ascii_names[keycode] == "Enter" && holded == 1 && !shifted)
    {
        shell_execute(input_buffer);
        return 1;
    }
    return 0;
}

bool input_check_backspace (uint8_t keycode, bool holded)
{
    if (ascii_names[keycode] == "Backspace" && holded == 1 && input_ptr > 0)
    {
        
        input_clear_last_char();
        terminal_update_cursor();
        return 1;
    }
    return 0;
}

bool input_check_caps (uint8_t keycode, bool holded)
{
    if (ascii_names[keycode] == "CapsLock" && holded == 1)
    {
        caps = !caps;
        return 1;
    }
    return 0;
}

bool input_check_shifting (uint8_t keycode, bool holded)
{
    if (ascii_names[keycode] == "LShift" || ascii_names[keycode] == "RShift")
    {
        shifted = holded;
        return 1;
    }
    return 0;
}

bool input_check_printable (uint8_t keycode, bool holded)
{
    for (int i = 0; i < (sizeof(not_printable) / sizeof(*not_printable)); i++)
    {
        if ((ascii_names[keycode] == not_printable[i]) || holded == 0)
        {
            return 0;
        }
    }
    return 1;
}

void input_print_char (uint8_t keycode)
{
    if (shifted && caps)
    {
        input_buffer[input_ptr] = ascii_shifted_caps[keycode];
        terminal_write_char(input_buffer[input_ptr]);
        input_ptr++;
        return;
    }
    if (caps)
    {
        input_buffer[input_ptr] = ascii_caps[keycode];
        terminal_write_char(input_buffer[input_ptr]);
        input_ptr++;
        return;
    }
    if (shifted)
    {
        input_buffer[input_ptr] = ascii_shifted[keycode];
        terminal_write_char(input_buffer[input_ptr]);
        input_ptr++;
        return;
    }

    input_buffer[input_ptr] = ascii_normal[keycode];
    terminal_write_char(input_buffer[input_ptr]);
    input_ptr++;

    return;
}
