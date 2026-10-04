#ifndef _DIAGNOSTIC_H
#define _DIAGNOSTIC_H

#ifdef __has_attribute
#if __has_attribute(__format__)
#define md2manc_printf(FORMAT_IDX, FIRST_ARG) \
    __attribute__((__format__(__printf__, FORMAT_IDX, FIRST_ARG)))
#endif
#endif

#ifndef md2manc_printf
#define md2manc_printf(FORMAT_IDX, FIRST_ARG)
#endif

/* Generate a warning but otherwise continue with conversion. Returns 0. */
int warning(const char *format, ...) md2manc_printf(1, 2);

/* Generate an error and stop execution. */
void error(const char *format, ...) md2manc_printf(1, 2);

#endif
