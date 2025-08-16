#include "commands.h"

inline static bool is_separator (const char *chr)
{
    if (!chr) return 0;
    switch (*chr)
    {
        case ' ': case '\t': case '\r': case '\n': return 1;
        default: return 0;
    }
}

static char *parse_quoted_value (char *p, const char **out)
{
    p++;
    *out = p;
    while (*p && *p != '"') p++;
    if (*p == '"')
    {
        *p = '\0';
        p++;
    }
    return p;
}

static char *parse_unquoted_value (char *p, const char **out)
{
    *out = p;
    while (*p && !is_separator(p)) p++;
    if (*p)
    {
        *p = '\0';
        p++;
    }
    return p;
}

static char *parse_flag (char *p, Command_Token_T *token)
{
    p += 2;
    token->type = TOKEN_BOOL_FLAG;
    token->name = p;
    token->value = "true";

    while (*p && !is_separator(p) && *p != '=' && *p != '"') p++;

    if (*p == '=')
    {
        *p = '\0';
        p++;
        token->type = TOKEN_TEXT_FLAG;
        if (*p == '"') p = parse_quoted_value(p, &token->value);
        else p = parse_unquoted_value(p, &token->value);
    }
    else if (*p == '"')
    {
        *p = '\0';
        p++;
        token->type = TOKEN_TEXT_FLAG;
        p = parse_quoted_value(p, &token->value);
    }
    else
    {
        if (*p) { *p = '\0'; p++; }
    }

    return p;
}

static char *parse_text (char *p, Command_Token_T *token)
{
    token->type = TOKEN_NORMAL;
    token->name = NULL;

    if (*p == '"') p = parse_quoted_value(p, &token->value);
    else p = parse_unquoted_value(p, &token->value);

    return p;
}

int commands_tokenize (char *input, Command_Token_T *tokens[])
{
    int token_count = 0;
    char *p = input;
    static Command_Token_T storage[MAX_TOKENS];

    while (*p && token_count < MAX_TOKENS)
    {
        while (is_separator(p)) p++;
        if (*p == '\0') break;

        Command_Token_T *token = &storage[token_count];
        token->name = NULL;
        token->value = NULL;

        if (p[0] == '-' && p[1] == '-') p = parse_flag(p, token);
        else p = parse_text(p, token);

        tokens[token_count++] = token;
    }

    tokens[token_count] = NULL;
    return token_count;
}

