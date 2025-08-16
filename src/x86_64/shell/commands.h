#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>
#include <stdint.h>
#include "strings.h"

#define MAX_TOKENS   32
#define MAX_COMMANDS 256

typedef enum {
    TOKEN_NORMAL = 0,
    TOKEN_BOOL_FLAG = 1,
    TOKEN_TEXT_FLAG = 2,
} Token_Types;

typedef struct {
    uint8_t  type;
    const char    *name;
    const char    *value;
} Command_Token_T;

typedef int (*command_func) (int argc, Command_Token_T **argv);

typedef struct {
    const char   *name;
    const char   *description;
    command_func  func;
} Command_Function_T;

extern const Command_Function_T __start_shell_commands[];
extern const Command_Function_T __stop_shell_commands[];

int commands_tokenize    (char *input, Command_Token_T *tokens[]);

#define SHELL_COMMAND(name_str, description_str, func_ptr) \
    __attribute__((used, section(".shell_commands"), aligned(sizeof(void*)))) \
    static const Command_Function_T __shell_command##func_ptr = { name_str, description_str, func_ptr };

#endif
