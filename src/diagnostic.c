#include "diagnostic.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int warning(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    fputs("warning: ", stderr);
    vfprintf(stderr, format, args);
    fputc('\n', stderr);

    va_end(args);
    return 0;
}

void error(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    fputs("error: ", stderr);
    vfprintf(stderr, format, args);
    fputc('\n', stderr);

    va_end(args);
}
