#ifndef SHELL_H
#define SHELL_H

#include "vga_print.h"
#include "strings.h"

#define INPUT_BUF_SIZE 255

extern bool input_allowed;

void shell_init    (void);
void shell_run     (void);
void shell_execute (char *input);

#endif