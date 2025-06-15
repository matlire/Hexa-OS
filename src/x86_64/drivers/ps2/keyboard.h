#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "vga_print.h"
#include "pic.h"
#include "strings.h"
#include "shell.h"

#define PS2_CMD   0x64
#define PS2_DATA  0x60

static const char *not_printable[] = {"ERROR", "Esc", "Backspace", "Tab", "LCtrl", "LShift", "RShift", "Keypad*", "LAlt", "CapsLock"};

static const char *ascii_names[] = { "ERROR",
    "Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace",
    "Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter",
    "LCtrl", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'",
    "`",
    "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "RShift",
    "Keypad*", "LAlt", "Space", "CapsLock"
};

static const char ascii_normal[] = { '?',
    '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 
    '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '`',
    '?', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?',
    '*',  '?', ' ', '?'
};

static const char ascii_shifted[] = { '?',
    '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '?',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 
    '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
    '~',
    '?', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '?',
    '*',  '?', ' ', '?'
};

static const char ascii_caps[] = { '?',
    '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 
    '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'',
    '`',
    '?', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', '?',
    '*',  '?', ' ', '?'
};

static const char ascii_shifted_caps[] = { '?',
    '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '?',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', 
    '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"',
    '~',
    '?', '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', '?',
    '*',  '?', ' ', '?'
};

static const char *ext_names[] = { "ERROR",
    "Insert", "Delete", "Home", "End", "PageUp", "PageDown",
    "ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight",
    "Keypad /", "Keypad Enter", "RCtrl", "RAlt",
    "WinLeft", "WinRight", "Menu"
};

static const char ext_normal[] = { '?',
    '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', '/', '\n', '?', '?', '?', '?', '?'
};

static const char ext_numed[] = { '?',
    '0', '.', '7', '1', '9', '3', '8', '2', '4', '6', '/', '\n', '?', '?', '?', '?', '5'
};

#define ASCII_SIZE (sizeof(ascii_names) / sizeof(*ascii_names))-1

static enum {
	KEY_UP 	 = 0,
	KEY_DOWN = 1
};

static uint8_t ascii_states[ASCII_SIZE] = {KEY_UP};
static uint8_t   ext_states[ASCII_SIZE] = {KEY_UP};

static bool    ext       = 0;
static uint8_t input_ptr = 0;
static char    input_buffer[INPUT_BUF_SIZE] = { EMPTY_CHAR_CHAR };

static bool shifted = 0;
static bool caps    = 0;
static bool numed   = 0;

static void   input_clear_buffer  (void);

static void   terminal_clear_char   (size_t col, size_t row);
static size_t input_last_row_char   (size_t row);
static void   input_clear_last_char (void);

static bool input_check_allowed   ();
static bool input_handle_keycode  (uint8_t keycode);

static bool input_check_enter 	  (uint8_t keycode, bool holded);
static bool input_check_backspace (uint8_t keycode, bool holded);
static bool input_check_caps 	  (uint8_t keycode, bool holded);
static bool input_check_shifting  (uint8_t keycode, bool holded);
static bool input_check_printable (uint8_t keycode, bool holded);
static void input_print_char      (uint8_t keycode);

void terminal_clear_char (size_t col, size_t row);
void handle_input        (uint8_t keycode);

#endif