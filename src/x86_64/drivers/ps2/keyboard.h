#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "vga_print.h"
#include "pic.h"
#include "strings.h"
#include "shell.h"
#include "chars.h"
#include "input_handlers.h"

#define PS2_CMD   0x64
#define PS2_DATA  0x60

static enum {
	KEY_UP 	 = 0,
	KEY_DOWN = 1
};

static uint8_t ascii_states[ASCII_SIZE] = {KEY_UP};
static uint8_t   ext_states[EXT_SIZE]   = {KEY_UP};

extern bool    ext;
extern uint8_t input_ptr;
extern char    input_buffer[INPUT_BUF_SIZE];

extern bool shifted;
extern bool caps;
extern bool numed;
extern bool ctrl;

extern size_t saved_m_col;

static int8_t ext_scancode_to_index (uint8_t keycode);

void   terminal_clear_char   	   (size_t col, size_t row);
size_t input_last_row_char   	   (size_t row);
void   update_input_last_data_row  (void);

static bool input_check_allowed     ();
static bool input_handle_keycode    (uint8_t keycode, bool ext);

static bool input_check_printable  (uint8_t keycode, bool holded);
static void input_print_char       (uint8_t keycode);

void handle_input          (uint8_t keycode);
void input_clear_buffer    (void);
void input_clear_last_char (void);
void input_redraw 		   (void);

#endif