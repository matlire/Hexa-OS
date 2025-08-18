#ifndef SHELL_H
#define SHELL_H

#include "vga_print.h"
#include "strings.h"
#include "input_handlers.h"
#include "queue.h"
#include "commands.h"
#include "time_core.h"

#define INPUT_BUF_SIZE 256

typedef struct {
    bool input_allowed;
    bool insert_mode;

    size_t input_start_col;
    size_t input_start_row;

    uint8_t input_ptr;
    char    input_buffer[INPUT_BUF_SIZE];
} Shell_State_T;

Shell_State_T* shell_get_state   (void);
void shell_update_input_buffer (uint8_t pos, char chr);
void shell_set_input_ptr       (uint8_t val);

void shell_set_input_allowed (bool val);
void shell_set_insert_mode   (bool val);

void shell_set_input_start_col (size_t val);
void shell_set_input_start_row (size_t val);

void shell_init    (void);
void shell_run     (void);
void shell_execute (char *input);

void shell_handle_input (void);

#endif
