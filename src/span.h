#ifndef _SPAN_H
#define _SPAN_H

#include <md4c.h>

/* Identifies text mode. */
enum text_mode {
    default_text,
    italic_text,
    bold_text,
    suppress_text
};

/* Pointer to the current text mode. */
extern enum text_mode *text_mode;

/* Emphasis. */
int enter_emphasis(void);
int leave_emphasis(void);

/* Strong. */
int enter_strong(void);
int leave_strong(void);

/* Link. */
int enter_link(MD_SPAN_A_DETAIL *detail);
int leave_link(void);

/* Image. */
int enter_image(void);
int leave_image(void);

#endif
