#include "input_handlers.h"

bool input_check_num(uint8_t keycode, bool holded)
{
	if (KEY_IS(keycode, "NumLock") && holded == 1)
	{
		numed = !numed;
		return 1;
	}
	return 0;
}

bool input_check_enter (uint8_t keycode, bool holded)
{
	if (KEY_IS(keycode, "Enter") && holded == 1 && !shifted)
	{
		shell_execute(input_buffer);
		input_clear_buffer();
		return 1;
	}
	return 0;
}

bool input_check_backspace (uint8_t keycode, bool holded)
{
	if (KEY_IS(keycode, "Backspace") && holded == 1 && input_ptr > 0)
	{
		void handle_remove (void)
		{
			input_ptr--;
			if (terminal_col == 0)
			{
				terminal_row--;
				terminal_col = input_last_row_char(terminal_row);
			} else {
				terminal_col--;
			}

			int i = input_ptr;
			size_t last_input_row = last_data_row;
			bool rem_n = 0;
			if (input_buffer[i] == '\n')
			{
				last_input_row--;
				rem_n = 1;
			}

			while (i < INPUT_BUF_SIZE-1)
			{
				input_buffer[i] = input_buffer[i+1];
				i++;
			}
			input_buffer[i] = '\0';
			
			input_redraw();
			if (rem_n && last_data_row == VGA_HEIGHT)
			{
				terminal_clear_row(last_data_row);
			}
			last_data_row = last_input_row;
		}
		if (ctrl)
		{
			while (input_ptr > 0 && is_word_sep(input_buffer[input_ptr - 1])) {
				handle_remove();
				input_ptr--;
			}
			while (input_ptr > 0 && !is_word_sep(input_buffer[input_ptr - 1])) {
				handle_remove();
			}
		} else {
			handle_remove();
		}
		terminal_update_cursor_pos();
		return 1;
	}
	return 0;
}

bool input_check_caps (uint8_t keycode, bool holded)
{
	if (KEY_IS(keycode, "CapsLock") && holded == 1)
	{
		caps = !caps;
		return 1;
	}
	return 0;
}

bool input_check_shift_ctrl (uint8_t keycode, bool holded)
{
	if (KEY_IS(keycode, "LShift") || KEY_IS(keycode, "RShift"))
	{
		shifted = holded;
		return 1;
	}
	if (KEY_IS(keycode, "LCtrl") || KEY_IS(keycode, "RCtrl"))
	{
		ctrl = holded;
		return 1;
	}

	return 0;
}

bool input_check_insert (uint8_t keycode, bool holded)
{
	if (KEY_IS(keycode, "Kp0") && holded)
	{
		insertm = !insertm;
		terminal_update_cursor_type(insertm);
		return 1;
	}
	return 0;
}

static bool is_word_sep(char c) {
	for (uint8_t i = 0; i < CTRL_SIZE; i++)
	{
		if (c == ctrl_break[i])
		{
			return 1;
		}
	}
    return 0;
}

bool input_ext_check_arrows (uint8_t keycode, bool holded)
{
	if (KEY_IS_EXT(keycode, "ArrowLeft") && holded && input_ptr > 0) {
	    if (ctrl) {
	        while (input_ptr > 0 && is_word_sep(input_buffer[input_ptr - 1])) {
	            if (terminal_col == 0) {
	                terminal_row--;
	                terminal_col = input_last_row_char(terminal_row);
	            } else {
	                terminal_col--;
	            }
	            input_ptr--;
	        }
	        while (input_ptr > 0 && !is_word_sep(input_buffer[input_ptr - 1])) {
	            if (terminal_col == 0) {
	                terminal_row--;
	                terminal_col = input_last_row_char(terminal_row);
	            } else {
	                terminal_col--;
	            }
	            input_ptr--;
	        }
	    } else {
	        if (terminal_col == 0) {
	            terminal_row--;
	            terminal_col = input_last_row_char(terminal_row);
	        } else {
	            terminal_col--;
	        }
	        input_ptr--;
	    }

	    terminal_update_cursor_pos();
	    saved_m_col = terminal_col;
	    return 1;
	}
	if (KEY_IS_EXT(keycode, "ArrowRight") && holded && input_ptr < strlen(input_buffer)) {
	    if (ctrl) {
	        while (input_ptr < strlen(input_buffer) && is_word_sep(input_buffer[input_ptr])) {
	            size_t last_col = input_last_row_char(terminal_row);
	            if (terminal_col == last_col && last_data_row == terminal_row) break;

	            if (terminal_col == last_col) {
	                terminal_row++;
	                terminal_col = 0;
	            } else {
	                terminal_col++;
	            }
	            input_ptr++;
	        }

	        while (input_ptr < strlen(input_buffer) && !is_word_sep(input_buffer[input_ptr])) {
	            size_t last_col = input_last_row_char(terminal_row);
	            if (terminal_col == last_col && last_data_row == terminal_row) break;

	            if (terminal_col == last_col) {
	                terminal_row++;
	                terminal_col = 0;
	            } else {
	                terminal_col++;
	            }
	            input_ptr++;
	        }
	    } else {
	        if (terminal_col == input_last_row_char(terminal_row) && last_data_row == terminal_row) {
	            return 1;
	        }

	        if (terminal_col == input_last_row_char(terminal_row)) {
	            terminal_row++;
	            terminal_col = 0;
	        } else {
	            terminal_col++;
	        }
	        input_ptr++;
	    }

	    terminal_update_cursor_pos();
	    saved_m_col = terminal_col;
	    return 1;
	}
	if (KEY_IS_EXT(keycode, "ArrowUp") && holded) {
		size_t saved_col = saved_m_col;
		size_t saved_row = terminal_row;
		uint8_t m = 0;
		while (input_ptr > 0 && terminal_row > (saved_row - 2))
		{
			if (terminal_col == 0)
			{
				m += 1;
				if (m == 2)
				{
					terminal_col = input_last_row_char(terminal_row);
					input_ptr += input_last_row_char(terminal_row);
					break;
				}
				terminal_row -= 1;
				terminal_col = input_last_row_char(terminal_row);
			} else {
				terminal_col -= 1;
			}
			input_ptr -= 1;
			if (terminal_col == saved_col)
			{
				break;
			}
		}
		terminal_update_cursor_pos();
		return 1;
	}

	if (KEY_IS_EXT(keycode, "ArrowDown") && holded) {
		size_t saved_col = saved_m_col;
		size_t saved_row = terminal_row;
		uint8_t m = 0;
		while (input_ptr < strlen(input_buffer) && terminal_row < (saved_row + 2))
		{
			if (terminal_col == input_last_row_char(terminal_row))
			{
				m += 1;
				if (m == 2)
				{
					break;
				}
				terminal_row += 1;
				terminal_col  = 0;
			} else {
				terminal_col += 1;
			}
			input_ptr += 1;
			if (terminal_col == saved_col)
			{
				break;
			}
		}
		terminal_update_cursor_pos();
		return 1;
	}

	return 0;
}

bool check_ext_del(uint8_t keycode, bool holded)
{
	size_t inp_len = strlen(input_buffer);
	if (KEY_IS_EXT(keycode, "Delete") && holded == 1 && input_ptr < inp_len)
	{
		void handle_remove (void) 
		{
			int i = input_ptr;
			size_t last_input_row = last_data_row;
			if (input_buffer[i+1] == '\n')
			{
				last_input_row--;
			}
			while (i < INPUT_BUF_SIZE-1)
			{
				input_buffer[i] = input_buffer[i+1];
				i++;
			}
			input_buffer[i] = '\0';
			
			input_redraw();
			last_data_row = last_input_row;
			terminal_update_cursor_pos();
		}

		if (ctrl)
		{
			int end = input_ptr;
			int buff_len = strlen(input_buffer);
			int saved_last_row = last_data_row;
			while (end < buff_len && is_word_sep(input_buffer[end])) {
		        end++;
		    }
		    while (end < buff_len && !is_word_sep(input_buffer[end])) {
		        end++;
		    }

		    int i = input_ptr;
		    int j = end;
		    while (input_buffer[i] != '\0') {
		    	if (j >= INPUT_BUF_SIZE)
		    	{
		    		input_buffer[i++] = '\0';
		    		continue;
		    	}
		        input_buffer[i++] = input_buffer[j++];
		    }
		    input_buffer[i] = '\0';

		    input_redraw();
		    last_data_row = saved_last_row;
		} else {
			handle_remove();
		}
		
		return 1;
	}
	return 0;
}

bool input_check_hend(uint8_t keycode, bool holded)
{
	if (KEY_IS(keycode, "Kp7") && holded) {
	    terminal_row = input_start_row;
	    terminal_col = input_start_col;
	    input_ptr = 0;
	    terminal_update_cursor_pos();
	    return 1;
	}
	if (KEY_IS(keycode, "Kp1") && holded) {
	    update_input_last_data_row();
	    input_ptr = strlen(input_buffer);
	    terminal_row = last_data_row;
	    terminal_col = input_last_row_char(last_data_row);
	    terminal_update_cursor_pos();
	    return 1;
	}
	return 0;
}
