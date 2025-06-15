#include "keyboard.h"


void handle_input (uint8_t keycode)
{
    if (!input_allowed || input_ptr >= INPUT_BUF_SIZE || input_ptr < 0)
    {
        return;
    }

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

    bool holded;
    if (keycode > ASCII_SIZE && ext == 0)
    {
        keycode -= 128;
        ascii_states[keycode] = 0;
        holded = 0;
    } else {
        ascii_states[keycode] = 1;
        holded = 1;
    }

    if (ascii_names[keycode] == "Backspace" && holded == 1 && input_ptr > 0)
    {
        input_buffer[input_ptr] = '\0';
        terminal_clear_last_char();
        input_ptr--;
        return;
    }

    if (ascii_names[keycode] == "CapsLock" && holded == 1)
    {
        caps = !caps;
        return;
    }

    if (ascii_names[keycode] == "LShift" || ascii_names[keycode] == "RShift")
    {
        if (holded)
        {
            shifted = 1;
        } else {
            shifted = 0;
        }
        return;
    }

    for (int i = 0; i < (sizeof(not_printable) / sizeof(*not_printable)); i++)
    {
        if ((ascii_names[keycode] == not_printable[i]) || holded == 0)
        {
            return;
        }
    }

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

void get_input_buffer (char *buffer)
{
    for (int i = 0; i < INPUT_BUF_SIZE; i++)
    {
        buffer[i] = input_buffer[i];
    }
    return;
}