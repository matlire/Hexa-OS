#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "vga_print.h"
#include "pic.h"
#include "strings.h"
#include "shell.h"
#include "chars.h"
#include "input_handlers.h"
#include "queue.h"

#define PS2_CMD   0x64
#define PS2_DATA  0x60

static enum {
	KEY_UP 	 = 0,
	KEY_DOWN = 1
};

typedef struct {
	Queue_T key_codes_queue;

	uint8_t ascii_states[KEYS_SIZE];
	uint8_t   ext_states[EXT_SIZE];

	bool waiting_ext;

	bool shift_holded;
	bool ctrl_holded;

	bool caps_active;
	bool num_pad_active;
} Keyboard_State_T;

void init_keyboard_state (void);

Keyboard_State_T* keyboard_get_state  (void);

void keyboard_set_waiting_ext    (bool val);
void keyboard_set_shift_holded   (bool val);
void keyboard_set_ctrl_holded    (bool val);
void keyboard_set_caps_active    (bool val);
void keyboard_set_num_pad_active (bool val);

void   terminal_clear_char   	   (size_t col, size_t row);
size_t input_last_row_char   	   (size_t row);
void   update_input_last_data_row  (void);

bool input_check_allowed     ();
bool input_handle_keycode    (uint8_t keycode, bool ext);

bool input_check_printable  (uint8_t keycode, bool holded);
void input_print_char       (uint8_t keycode);

void queue_input           (uint8_t keycode);
void input_clear_buffer    (void);
void input_clear_last_char (void);
void input_redraw 		   (void);

#endif
