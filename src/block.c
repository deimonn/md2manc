#include "block.h"

#include "diagnostic.h"

#include <md4c.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define MAX_BLOCK_NESTING 32

//==============//
// Shared state //
//==============//

static enum block_mode block_mode_stack[MAX_BLOCK_NESTING];
enum block_mode *block_mode = block_mode_stack;

static bool tight_list_stack[MAX_BLOCK_NESTING];
static bool *tight_list = tight_list_stack - 1;

static int skip_paragraph_instructions = 0;

bool in_synopsis_section = false;
int in_heading_block = 0;

static void push_block_mode(enum block_mode mode)
{
    if (block_mode - block_mode_stack > MAX_BLOCK_NESTING) {
        error("too many nested blocks");
        exit(1);
    }

    if (*block_mode != default_block)
        printf(".RS\n");

    *++block_mode = mode;
}

static void pop_block_mode(void)
{
    block_mode--;

    if (*block_mode != default_block)
        printf(".RE\n");
}

//=============//
// Block quote //
//=============//

int enter_block_quote(void)
{
    push_block_mode(indented_block);
    return 0;
}

int leave_block_quote(void)
{
    pop_block_mode();
    return 0;
}

//================//
// Unordered list //
//================//

int enter_unordered_list(MD_BLOCK_UL_DETAIL *detail)
{
    if (detail->mark == '+' && !detail->is_tight) {
        push_block_mode(definition_list_block);
        return 0;
    }

    push_block_mode(unordered_list_block);

    *++tight_list = detail->is_tight;

    return 0;
}

int leave_unordered_list(void)
{
    tight_list--;

    pop_block_mode();

    skip_paragraph_instructions = 0;
    return 0;
}

//==============//
// Ordered list //
//==============//

static int list_cardinal_stack[MAX_BLOCK_NESTING];
static int *list_cardinal = list_cardinal_stack - 1;

int enter_ordered_list(MD_BLOCK_OL_DETAIL *detail)
{
    push_block_mode(ordered_list_block);

    *++tight_list = detail->is_tight;
    *++list_cardinal = detail->start;

    return 0;
}

int leave_ordered_list(void)
{
    tight_list--;
    list_cardinal--;

    pop_block_mode();

    skip_paragraph_instructions = 0;
    return 0;
}

//===========//
// List item //
//===========//

int enter_list_item(void)
{
    if (*block_mode == unordered_list_block) {
        printf(".IP \\(bu 2n\n");
        skip_paragraph_instructions = 1;
    } else if (*block_mode == ordered_list_block) {
        printf(".IP (%i) 7n\n", (*list_cardinal)++);
        skip_paragraph_instructions = 1;
    } else if (*block_mode == definition_list_block) {
        printf(".TP 7n\n");
        skip_paragraph_instructions = 2;
    } else {
        assert(!"invalid list item block type");
    }

    return 0;
}

int leave_list_item(void)
{
    if (*block_mode == definition_list_block && skip_paragraph_instructions) {
        warning("missing definition in definition list");
        printf("\\&\n");
    }

    if (*tight_list)
        printf("\n");

    skip_paragraph_instructions = 0;
    return 0;
}

//=========//
// Heading //
//=========//

int enter_heading(MD_BLOCK_H_DETAIL *detail)
{
    if (detail->level == 1) {
        printf(".SH ");
    } else if (detail->level == 2) {
        printf(".SS ");
    } else {
        printf(".PP\n");
        printf(".B ");
        warning("heading level %i unsupported", detail->level);
    }

    in_heading_block = detail->level;
    return 0;
}

int leave_heading(void)
{
    if (in_heading_block > 2) {
        printf("\n.PP\n");
    } else {
        printf("\n");
    }

    in_heading_block = 0;
    skip_paragraph_instructions = 1;

    return 0;
}

//============//
// Code block //
//============//

int enter_code_block(MD_BLOCK_CODE_DETAIL *detail)
{
    if (in_synopsis_section) {
        MD_ATTRIBUTE lang = detail->lang;

        if (strncasecmp("C", lang.text, lang.size) == 0)
            push_block_mode(c_code_block);
        else if (strncasecmp("Usage", lang.text, lang.size) == 0)
            push_block_mode(usage_code_block);
        else
            push_block_mode(default_block);
    } else {
        push_block_mode(indented_block);
        printf(".IP \\& 7n\n");
    }

    printf(".nf\n");
    return 0;
}

int leave_code_block(void)
{
    pop_block_mode();
    printf(".fi\n");
    return 0;
}

//===========//
// Paragraph //
//===========//

int enter_paragraph(void)
{
    if (skip_paragraph_instructions) {
        skip_paragraph_instructions--;
        return 0;
    }

    if (*block_mode != default_block)
        printf(".IP \\& 7n\n");
    else
        printf(".PP\n");

    return 0;
}

int leave_paragraph(void)
{
    printf("\n");
    return 0;
}
