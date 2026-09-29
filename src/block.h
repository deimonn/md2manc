#ifndef _BLOCK_H
#define _BLOCK_H

#include <stdbool.h>

#include <md4c.h>

/* Identifies block mode. */
enum block_mode {
    default_block,
    indented_block,
    ordered_list_block,
    unordered_list_block,
    definition_list_block,
    c_code_block,
    usage_code_block
};

/* Pointer to the mode of the current block. */
extern enum block_mode *block_mode;
/* Tracks whether we're currently in a heading block and which level it is. */
extern int in_heading_block;
/* Whether the current block is below a SYNOPSIS heading. */
extern bool in_synopsis_section;

/* Block quote. */
int enter_block_quote(void);
int leave_block_quote(void);

/* Unordered list. */
int enter_unordered_list(MD_BLOCK_UL_DETAIL *detail);
int leave_unordered_list(void);

/* Ordered list. */
int enter_ordered_list(MD_BLOCK_OL_DETAIL *detail);
int leave_ordered_list(void);

/* List item. */
int enter_list_item(void);
int leave_list_item(void);

/* Heading. */
int enter_heading(MD_BLOCK_H_DETAIL *detail);
int leave_heading(void);

/* Code block. */
int enter_code_block(MD_BLOCK_CODE_DETAIL *detail);
int leave_code_block(void);

/* Paragraph. */
int enter_paragraph(void);
int leave_paragraph(void);

/* Table. */
int enter_paragraph(void);
int leave_paragraph(void);

#endif
