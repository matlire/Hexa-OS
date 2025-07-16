#include "keyboard.h"

bool    ext    = 0;
uint8_t input_ptr = 0;
char    input_buffer[INPUT_BUF_SIZE] = { EMPTY_CHAR_CHAR };

bool shifted = 0;
bool caps   = 0;
bool numed   = 0;
bool ctrl   = 0;

size_t saved_m_col = 0;

void handle_input (uint8_t keycode)
{
    if (!input_check_allowed()) { return; }

    if (keycode == 0xe0)
    {
        ext = 1;
        return;
    }
    if (ext)
    {
        bool holded = input_handle_keycode(keycode, 1);
        if (holded == 0) { keycode -= 128; }
        keycode = ext_scancode_to_index(keycode);

        input_ext_check_arrows(keycode, holded);
        check_ext_del(keycode, holded);
        ext = 0;
        return;
    }

    if (keycode >= KEYS_SIZE || keycode <= 128)
    {
        return;
    }

    bool holded = input_handle_keycode(keycode, 0);
    if (holded == 0) { keycode -= 128; }

    if (input_check_num(keycode, holded)) { return; }

    if (input_check_insert(keycode, holded)) { return;}

    if (input_check_enter(keycode, holded)) { return; }

    if (input_check_backspace(keycode, holded)) { return; }

    if (input_check_caps(keycode, holded)) { return; }

    if (input_check_shift_ctrl(keycode, holded)) { return; }

    if (input_check_hend(keycode, holded)) { return; }

    if (input_check_printable(keycode, holded))
    {
        input_print_char(keycode);
    }

    return;
}

static int8_t ext_scancode_to_index(uint8_t keycode) {
    switch (keycode) {
        case 0x00: return  0;  // “ERROR”
        case 0x52: return  1;  // Insert
        case 0x53: return  2;  // Delete
        case 0x47: return  3;  // Home
        case 0x4F: return  4;  // End
        case 0x49: return  5;  // PageUp
        case 0x51: return  6;  // PageDown
        case 0x48: return  7;  // ArrowUp
        case 0x50: return  8;  // ArrowDown
        case 0x4B: return  9;  // ArrowLeft
        case 0x4D: return 10;  // ArrowRight
        case 0x35: return 11;  // Keypad /
        case 0x1C: return 12;  // Keypad Enter
        case 0x1D: return 13;  // RCtrl
        case 0x38: return 14;  // RAlt
        case 0x5B: return 15;  // WinLeft
        case 0x5C: return 16;  // WinRight
        case 0x5D: return 17;  // Menu
        default:   return -1;  // not in our list
    }
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
    saved_m_col = terminal_col;
    input_start_row = terminal_row;
    input_start_col = terminal_col;
    return;
}


void input_clear_last_char (void)
{
    if (input_ptr == 0) return;
    input_ptr--;
    saved_m_col--;
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
    return;
}

void update_input_last_data_row (void)
{
    last_data_row = input_start_row + count(input_buffer, '\n');
    return;
}

static bool input_check_allowed ()
{
    if (!input_allowed || input_ptr >= INPUT_BUF_SIZE)
    {
        return 0;
    }
    return 1;
}

static bool input_handle_keycode (uint8_t keycode, bool ext)
{
    bool new_state;
    if (keycode > KEYS_SIZE)
    {
        keycode -= 128;
        new_state = 0;
    } else {
        new_state = 1;
    }
    if (ext)
    {
        ext_scancode_to_index(keycode);
        ext_states[keycode] = new_state;
    } else {
        ascii_states[keycode] = new_state;
    }
    return new_state;
}

static bool input_check_printable (uint8_t keycode, bool holded)
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

void input_redraw (void)
{
    size_t saved_row = terminal_row;
    size_t saved_col = terminal_col;
    terminal_row = input_start_row;
    terminal_col = input_start_col;
    int i = 0;
    while (i < INPUT_BUF_SIZE-1)
    {
        if (input_buffer[i] == '\0')
        {
            terminal_write_char(EMPTY_CHAR_CHAR);
            if (terminal_row == last_data_row + 1 || (terminal_row == VGA_HEIGHT - 1 && terminal_col == VGA_WIDTH - 1))
            {
                break;
            }
        }
        if (input_buffer[i] == '\n')
        {
            for (size_t j = terminal_col + 1; j < VGA_WIDTH; j++)
            {
                terminal_write_char(EMPTY_CHAR_CHAR);
            }
            terminal_col = 0;
            terminal_write_char('\n');
        } else if (input_buffer[i] != '\0') {
            terminal_write_char(input_buffer[i]);
        }
        i++;
    }
    terminal_col = saved_col;
    terminal_row = saved_row;
    terminal_update_cursor_pos();
}

static void input_print_char (uint8_t keycode)
{
    char c;
    if (shifted && caps)       c = ascii_shifted_caps[keycode];
    else if (caps)             c = ascii_caps[keycode];
    else if (shifted)          c = ascii_shifted[keycode];
    else                       c = ascii_normal[keycode];
    
    if (!insertm)
    {
        input_buffer[input_ptr++] = c;
        terminal_write_char(c);
        saved_m_col = terminal_col;
        input_redraw();
    } else {
        int i = INPUT_BUF_SIZE-1;
        while (i > input_ptr)
        {
            input_buffer[i] = input_buffer[i-1];
            i--;
        }
        input_buffer[input_ptr] = c;
        input_ptr++;
        i = 0;
        size_t saved_row = terminal_row;
        size_t saved_col = terminal_col;
        terminal_col = input_start_col;
        terminal_row = input_start_row;
        while (i < strlen(input_buffer))
        {
            terminal_write_char(input_buffer[i++]);
        }
        if (c != '\n')
        {
            terminal_row = saved_row;
            terminal_col = saved_col + 1;
        } else {
            terminal_col = 0;
            input_redraw();
        }
        saved_m_col = terminal_col;
    }
    update_input_last_data_row();
    terminal_update_cursor_pos();
    return;
}
