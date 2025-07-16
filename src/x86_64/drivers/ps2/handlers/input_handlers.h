#ifndef INPUT_HANDLERS_H
#define INPUT_HANDLERS_H

#include "vga_print.h"
#include "pic.h"
#include "strings.h"
#include "shell.h"
#include "chars.h"
#include "keyboard.h"

#define KEY_IS(code, name)     (strcmp(ascii_names[code], name) == 0)
#define KEY_IS_EXT(code, name) (strcmp(ext_names[code]  , name) == 0)

static bool is_word_sep(char c);

bool input_check_num		(uint8_t keycode, bool holded);
bool input_check_enter 	    (uint8_t keycode, bool holded);
bool input_check_backspace  (uint8_t keycode, bool holded);
bool input_check_caps 	    (uint8_t keycode, bool holded);
bool input_check_shift_ctrl (uint8_t keycode, bool holded);
bool input_check_insert 	(uint8_t keycode, bool holded);
bool input_check_hend       (uint8_t keycode, bool holded);

bool input_ext_check_arrows (uint8_t keycode, bool holded);
bool check_ext_del			(uint8_t keycode, bool holded);

#endif