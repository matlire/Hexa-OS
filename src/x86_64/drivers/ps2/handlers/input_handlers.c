#include "input_handlers.h"

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

bool input_check_num (uint8_t keycode, bool holded)
{
	if (keycode == KEY_NUMLOCK && holded == 1)
	{
		keyboard_set_num_pad_active(!keyboard_get_state()->num_pad_active);
		return 1;
	}

	return 0;
}

bool input_check_enter (uint8_t keycode, bool holded)
{
	if (keycode == KEY_ENTER && holded == 1 && !keyboard_get_state()->shift_holded)
	{
		shell_execute(shell_get_state()->input_buffer);
		input_clear_buffer();
		return 1;
	}

	return 0;
}

bool input_check_backspace (uint8_t keycode, bool holded)
{
	uint8_t input_ptr = shell_get_state()->input_ptr;
	if (keycode == KEY_BACKSPACE && holded == 1 && input_ptr > 0)
	{
		void handle_remove (void)
		{
			input_ptr -= 1;
			shell_set_input_ptr(input_ptr);
			if (terminal_get_state()->column == 0)
			{
				terminal_set_row(terminal_get_state()->row - 1);
				terminal_set_column(input_last_row_char(terminal_get_state()->row));
			} else {
				terminal_set_column(terminal_get_state()->column - 1);
			}

			int i = input_ptr;
			size_t last_input_row = terminal_get_state()->last_row;
			size_t last_row = terminal_get_state()->last_row;
			bool rem_n = 0;
			if (shell_get_state()->input_buffer[i] == '\n')
			{
				last_input_row--;
				rem_n = 1;
			}

			while (i < INPUT_BUF_SIZE - 1)
			{
				shell_update_input_buffer(i, shell_get_state()->input_buffer[i+1]);
				i++;
			}
			shell_update_input_buffer(i, '\0');
			
			input_redraw();
			if (rem_n && last_row == VGA_HEIGHT)
			{
				terminal_clear_row(last_row);
			}
			terminal_set_last_row(last_input_row);
		}
		if (keyboard_get_state()->ctrl_holded)
		{
			while (input_ptr > 0 && !is_word_sep(shell_get_state()->input_buffer[shell_get_state()->input_ptr - 1])) {
				handle_remove();
			}
			if (input_ptr > 0 && is_word_sep(shell_get_state()->input_buffer[shell_get_state()->input_ptr - 1])) {
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
	if (keycode == KEY_CAPSLOCK && holded == 1)
	{
		keyboard_set_caps_active(keyboard_get_state()->caps_active);
		return 1;
	}

	return 0;
}

bool input_check_shift_ctrl (uint8_t keycode, bool holded)
{
	if (keycode == KEY_LSHIFT || keycode == KEY_RSHIFT)
	{
		keyboard_set_shift_holded(holded);
		return 1;
	}
	if (keycode == KEY_LCTRL || keycode == KEY_RCTRL)
	{
		keyboard_set_ctrl_holded(holded);
		return 1;
	}

	return 0;
}

bool input_check_insert (uint8_t keycode, bool holded)
{
	if (keycode == KEY_KP0 && holded)
	{
		shell_set_insert_mode(!shell_get_state()->insert_mode);
		terminal_update_cursor_type(shell_get_state()->insert_mode);
		return 1;
	}

	return 0;
}

static bool input_ext_check_left_arrow (uint8_t keycode, bool holded)
{
	uint8_t input_ptr = shell_get_state()->input_ptr;
	if (keycode == KEY_ARROW_LEFT && holded && input_ptr > 0) {
		size_t column = terminal_get_state()->column;
		size_t row 	  = terminal_get_state()->row;
	    if (keyboard_get_state()->ctrl_holded) {
	        while (input_ptr > 0 && is_word_sep(shell_get_state()->input_buffer[input_ptr - 1])) {
	            if (column == 0) {
	                row -= 1;
	                column = input_last_row_char(row);
	            } else {
	                column -= 1;
	            }
	            input_ptr -= 1;
	        }
	        while (input_ptr > 0 && !is_word_sep(shell_get_state()->input_buffer[input_ptr - 1])) {
	            if (column == 0) {
	                row -= 1;
	                column = input_last_row_char(row);
	            } else {
	                column -= 1;
	            }
	            input_ptr -= 1;
	        }
	        shell_set_input_ptr(input_ptr);
	    } else {
	        if (column == 0) {
	            row -= 1; 
	            column = input_last_row_char(row);
	        } else {
	            column -= 1;
	        }
	        shell_set_input_ptr(input_ptr - 1);
	    }

	    terminal_set_column(column);
	    terminal_set_row(row);
	    terminal_update_cursor_pos();
	    return 1;
	}

	return 0;
}

static bool input_ext_check_right_arrow (uint8_t keycode, bool holded)
{
	uint8_t input_ptr = shell_get_state()->input_ptr;
	if (keycode == KEY_ARROW_RIGHT && holded && input_ptr < strlen(shell_get_state()->input_buffer)) {
		size_t column = terminal_get_state()->column;
		size_t row 	  = terminal_get_state()->row;
	    if (keyboard_get_state()->ctrl_holded) {
	        while (input_ptr < strlen(shell_get_state()->input_buffer) && is_word_sep(shell_get_state()->input_buffer[input_ptr])) {
	            size_t last_col = input_last_row_char(row);
	            if (column == last_col && terminal_get_state()->last_row == row) break;

	            if (column == last_col) {
	            	row += 1;;
	                column = 0;
	            } else {
	                column += 1;;
	            }
	            input_ptr += 1;
	        }

	        while (input_ptr < strlen(shell_get_state()->input_buffer) && !is_word_sep(shell_get_state()->input_buffer[input_ptr])) {
	            size_t last_col = input_last_row_char(row);
	            if (column == last_col && row == terminal_get_state()->row) break;

	            if (column == last_col) {
	                row += 1;;
	                column = 0;
	            } else {
	                column += 1;
	            }
	            input_ptr += 1;
	        }

	        shell_set_input_ptr(input_ptr);
	    } else {
	        if (column == input_last_row_char(column) && terminal_get_state()->last_row == row) {
	            return 1;
	        }

	        if (column == input_last_row_char(row)) {
	            row += 1;
	            column = 0;
	        } else {
	            column += 1;
	        }
	        shell_set_input_ptr(input_ptr + 1);
	    }

	    terminal_set_column(column);
	    terminal_set_row(row);
	    terminal_update_cursor_pos();

	    return 1;
	}

	return 0;
}

static bool input_ext_check_up_arrow    (uint8_t keycode, bool holded)
{
	uint8_t input_ptr = shell_get_state()->input_ptr;
	if (keycode == KEY_ARROW_UP && holded) {
		size_t saved_col = terminal_get_state()->column;
		size_t column = terminal_get_state()->column;
		size_t row 	  = terminal_get_state()->row;
		size_t saved_row = terminal_get_state()->row;
		uint8_t m = 0;
		while (input_ptr > 0 && row > (saved_row - 2))
		{
			if (column == 0)
			{
				m += 1;
				if (m == 2)
				{
					column = input_last_row_char(row);
					input_ptr += input_last_row_char(row);
					break;
				}
				row -= 1;
				column = input_last_row_char(row);
			} else {
				column -= 1;
			}
			input_ptr -= 1;
			if (column == saved_col || (column <= saved_col && m > 0))
			{
				break;
			}
		}

		terminal_set_column(column);
		terminal_set_row(row);
		shell_set_input_ptr(input_ptr);
		terminal_update_cursor_pos();

		return 1;
	}

	return 0;
}

static bool input_ext_check_down_arrow  (uint8_t keycode, bool holded)
{
	uint8_t input_ptr = shell_get_state()->input_ptr;
	if (keycode == KEY_ARROW_DOWN && holded) {
		size_t saved_col = terminal_get_state()->column;
		size_t saved_row = terminal_get_state()->row;
		size_t column = terminal_get_state()->column;
		size_t row 	  = terminal_get_state()->row;
		uint8_t m = 0;
		while (input_ptr < strlen(shell_get_state()->input_buffer) && row < (saved_row + 2))
		{
			if (column == input_last_row_char(row))
			{
				m += 1;
				if (m == 2)
				{
					break;
				}
				row += 1;
				column = 0;
			} else {
				column += 1;
			}
			input_ptr += 1;
			if (column == saved_col)
			{
				break;
			}
		}

		terminal_set_column(column);
		terminal_set_row(row);
		shell_set_input_ptr(input_ptr);
		terminal_update_cursor_pos();

		return 1;
	}

	return 0;
}

bool input_ext_check_arrows (uint8_t keycode, bool holded)
{
	if (input_ext_check_left_arrow(keycode, holded) || input_ext_check_right_arrow(keycode, holded) || input_ext_check_up_arrow(keycode, holded) || input_ext_check_down_arrow(keycode, holded)) {
		return 1;
	}

	return 0;
}

bool check_ext_del(uint8_t keycode, bool holded)
{
	size_t inp_len = strlen(shell_get_state()->input_buffer);
	if (keycode == KEY_DELETE && holded == 1 && shell_get_state()->input_ptr < inp_len)
	{
		void handle_remove (void) 
		{
			int i = shell_get_state()->input_ptr;
			size_t last_input_row = terminal_get_state()->last_row;
			if (shell_get_state()->input_buffer[i + 1] == '\n')
			{
				last_input_row--;
			}
			while (i < INPUT_BUF_SIZE - 1)
			{
				shell_update_input_buffer(i, shell_get_state()->input_buffer[i+1]);
				i++;
			}
			shell_update_input_buffer(i, '\0');
			
			input_redraw();
			terminal_set_last_row(last_input_row);
			terminal_update_cursor_pos();
		}

		if (keyboard_get_state()->ctrl_holded)
		{
			int end = shell_get_state()->input_ptr;
			int buff_len = strlen(shell_get_state()->input_buffer);
			int saved_last_row = terminal_get_state()->last_row;
			if (end < buff_len && is_word_sep(shell_get_state()->input_buffer[end])) {
		        end++;
		    }
		    while (end < buff_len && !is_word_sep(shell_get_state()->input_buffer[end])) {
		        end++;
		    }

		    int i = shell_get_state()->input_ptr;
		    int j = end;
		    while (shell_get_state()->input_buffer[i] != '\0') {
		    	if (j >= INPUT_BUF_SIZE)
		    	{
		    		shell_update_input_buffer(i++, '\0');
		    		continue;
		    	}
		    	shell_update_input_buffer(i++, shell_get_state()->input_buffer[j++]);
		    }
		    shell_update_input_buffer(i, '\0');

		    input_redraw();
		    terminal_set_last_row(saved_last_row);
		} else {
			handle_remove();
		}
		
		return 1;
	}

	return 0;
}

bool input_check_hend(uint8_t keycode, bool holded)
{
	if (keycode == KEY_KP7 && holded && keyboard_get_state()->num_pad_active) {
	    terminal_set_row(shell_get_state()->input_start_row);
	    terminal_set_column(shell_get_state()->input_start_col);
	    shell_set_input_ptr(0);
	    terminal_update_cursor_pos();

	    return 1;
	}
	if (keycode == KEY_KP1 && holded && keyboard_get_state()->num_pad_active) {
	    update_input_last_data_row();
	    shell_set_input_ptr(strlen(shell_get_state()->input_buffer));
	    terminal_set_row(terminal_get_state()->last_row);
	    terminal_set_column(input_last_row_char(terminal_get_state()->last_row));
	    terminal_update_cursor_pos();
	    
	    return 1;
	}
	return 0;
}
