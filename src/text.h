#ifndef _TEXT_H
#define _TEXT_H

#include <md4c.h>

/* Print normal text. */
int print_text(const char *text, unsigned int size);

/* Print null character. */
int print_nullchar(void);

/* Print hard break. */
int print_hard_break(void);

/* Print soft break. */
int print_soft_break(void);

/* Parse and print HTML entity. */
int print_entity(const char *text, unsigned int size);

/* Process and print code. */
int print_code(const char *text, unsigned int size);

#endif
