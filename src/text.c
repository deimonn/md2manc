#include "text.h"

#include "block.h"
#include "diagnostic.h"
#include "entities.h"
#include "span.h"

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAX_HIGHLIGHT_LINE_LENGTH 1024

//======//
// Util //
//======//

static void set_font(void)
{
    if (*text_mode == italic_text)
        printf("\\fI");
    else if (*text_mode == bold_text)
        printf("\\fB");
}

static void reset_font(void)
{
    if (*text_mode == italic_text || *text_mode == bold_text)
        printf("\\fP");
}

static void escape_text(const char *text, unsigned int size)
{
    if (*text == '.')
        printf("\\&");

    for (unsigned int i = 0; i < size; i++) {
        int c = text[i];

        if (c == '-') {
            printf("\\-");
            continue;
        }

        if (c == '"') {
            printf("\\(dq");
            continue;
        }

        if (c == '\\') {
            printf("\\e");
            continue;
        }

        putchar(c);
    }
}

//==============//
// Highlighting //
//==============//

static char line[MAX_HIGHLIGHT_LINE_LENGTH];
static unsigned int line_length = 0;

static void append_to_line(const char *text, unsigned int size)
{
    if (line_length + size > MAX_HIGHLIGHT_LINE_LENGTH) {
        error("line in syntax highlighted code is too long (≥1 KiB)");
        exit(1);
    }

    memcpy(line + line_length, text, size);
    line_length += size;
}

static void clear_line(void)
{
    line_length = 0;
}

static bool is_identifier_starter(int c)
{
    return isalpha(c) || c == '_';
}

static bool is_identifier(int c)
{
    return isalnum(c) || c == '_';
}

static int print_c_code(const char *text, unsigned int size)
{
    append_to_line(text, size);

    if (text[size - 1] != '\n')
        return 0;

    if (*line == '.')
        printf("\\&");

    bool in_token = false;
    bool is_variable = line_length >= 7 && memcmp(line, "extern ", 7) == 0;
    bool is_typedef = line_length >= 8 && memcmp(line, "typedef ", 8) == 0;
    bool follows_struct_or_union_kw = false;

    for (unsigned int i = 0; i < line_length; i++) {
        int cc = line[i];

        if (!in_token && is_identifier_starter(cc)) {
            if (follows_struct_or_union_kw) {
                follows_struct_or_union_kw = false;
                in_token = true;
                printf("\\fB");
                putchar(cc);
                continue;
            }

            unsigned int wb = i + 1;

            while (wb < line_length) {
                if (!is_identifier(line[wb]) && !strchr("[]", line[wb]))
                    break;

                wb++;
            }

            unsigned int len = wb - i;

            if (len == 4 && memcmp(line + i, "void", len) == 0) {
                printf("void");
                i += len - 1;
                continue;
            }

            if (len == 5 && memcmp(line + i, "union", len) == 0) {
                printf("union");
                follows_struct_or_union_kw = true;
                i += len - 1;
                continue;
            }

            if (len == 6 && memcmp(line + i, "struct", len) == 0) {
                printf("struct");
                follows_struct_or_union_kw = true;
                i += len - 1;
                continue;
            }

            if (wb < line_length) {
                if (strchr("(", line[wb])) {
                    in_token = true;
                    printf("\\fB");
                } else if (strchr(",)", line[wb])) {
                    in_token = true;
                    printf("\\fI");
                } else if (line[wb] == ';') {
                    if (is_variable || is_typedef) {
                        in_token = true;
                        printf("\\fB");
                    } else {
                        in_token = true;
                        printf("\\fI");
                    }
                }
            }

            putchar(cc);
            continue;
        }

        if (!isspace(cc))
            follows_struct_or_union_kw = false;

        if (in_token && (isspace(cc) || strchr(",;()[]{}", cc))) {
            in_token = false;
            printf("\\fP");
            putchar(cc);
            continue;
        }

        if (cc == '-') {
            printf("\\-");
            continue;
        }

        if (cc == '\\') {
            printf("\\e");
            continue;
        }

        putchar(cc);
    }

    clear_line();
    return 0;
}

static bool is_usage_separator(int c)
{
    return isspace(c) || strchr("[]|.", c);
}

static int print_usage_code(const char *text, unsigned int size)
{
    append_to_line(text, size);

    if (text[size - 1] != '\n')
        return 0;

    bool in_token = false;

    if (!isspace(*line)) {
        printf("\\fB");
        in_token = true;
    }

    int pc;
    int cc = '\n';

    for (unsigned int i = 0; i < line_length; i++) {
        pc = cc;
        cc = line[i];

        if (i && in_token && is_usage_separator(cc)) {
            in_token = false;
            printf("\\fP");
        }

        if (cc == '-') {
            if (!in_token && is_usage_separator(pc)) {
                in_token = true;
                printf("\\fB");
            }

            printf("\\-");
            continue;
        }

        if (isalpha(cc)) {
            if (!in_token && is_usage_separator(pc)) {
                in_token = true;
                printf("\\fI");
            }

            putchar(cc);
            continue;
        }

        if (cc == '\\') {
            printf("\\e");
            continue;
        }

        putchar(cc);
    }

    clear_line();
    return 0;
}

//======//
// Text //
//======//

int print_text(const char *text, unsigned int size)
{
    if (in_heading_block == 1)
        in_synopsis_section = strncasecmp(text, "SYNOPSIS", size) == 0;

    set_font();
    escape_text(text, size);
    reset_font();

    return 0;
}

int print_nullchar(void)
{
    set_font();
    printf("\\[uFFFD]");
    reset_font();

    return 0;
}

int print_hard_break(void)
{
    if (*block_mode != default_block)
        printf(".IP\n");
    else
        printf(".PP\n");

    return 0;
}

int print_soft_break(void)
{
    printf(" ");
    return 0;
}

int print_entity(const char *text, unsigned int size)
{
    char entity[48];

    if (size > sizeof(entity) - 1)
        return print_nullchar();

    memcpy(entity, text, size);
    entity[size] = '\0';

    set_font();

    if (strncmp(entity, "&#x", 3) == 0)
        printf("\\[u%04lX]", strtoul(entity + 3, NULL, 16));
    else if (strncmp(entity, "&#", 2) == 0)
        printf("\\[u%04lX]", strtoul(entity + 2, NULL, 10));
    else if (parse_named_entity(entity) != 0)
        warning("invalid HTML named entity: %s", entity);

    reset_font();
    return 0;
}

int print_code(const char *text, unsigned int size)
{
    if (*block_mode == c_code_block)
        return print_c_code(text, size);
    if (*block_mode == usage_code_block)
        return print_usage_code(text, size);

    set_font();
    escape_text(text, size);
    reset_font();

    return 0;
}
