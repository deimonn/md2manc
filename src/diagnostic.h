#ifndef _DIAGNOSTIC_H
#define _DIAGNOSTIC_H

#ifdef __has_attribute
#if __has_attribute(__format__) && !defined(__format)
#define __format(...) __attribute__((__format__(__VA_ARGS__)))
#endif
#endif

#ifndef __format
#define __format(...)
#endif

/* Generate a warning but otherwise continue with conversion. Returns 0. */
int warning(const char *format, ...) __format(printf, 1, 2);

/* Generate an error and stop execution. */
void error(const char *format, ...) __format(printf, 1, 2);

#endif
