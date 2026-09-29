#include "span.h"

#include "diagnostic.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SPAN_NESTING 32

//==============//
// Shared state //
//==============//

static enum text_mode text_mode_stack[MAX_SPAN_NESTING];
enum text_mode *text_mode = text_mode_stack;

static void push_text_mode(enum text_mode mode)
{
    if (text_mode - text_mode_stack > MAX_SPAN_NESTING) {
        error("too many nested spans");
        exit(1);
    }

    if (*text_mode == suppress_text)
        *++text_mode = suppress_text;
    else
        *++text_mode = mode;
}

static void pop_text_mode(void)
{
    text_mode--;
}

//==========//
// Emphasis //
//==========//

int enter_emphasis(void)
{
    push_text_mode(italic_text);
    return 0;
}

int leave_emphasis(void)
{
    pop_text_mode();
    return 0;
}

//========//
// Strong //
//========//

int enter_strong(void)
{
    push_text_mode(bold_text);
    return 0;
}

int leave_strong(void)
{
    pop_text_mode();
    return 0;
}

//======//
// Link //
//======//

enum link_type {
    relative_link,
    absolute_link,
    mail_address,
};

static enum link_type link_type;
static bool is_autolink;

int enter_link(MD_SPAN_A_DETAIL *detail)
{
    MD_ATTRIBUTE href = detail->href;

    if (!memchr(href.text, ':', href.size)) {
        link_type = relative_link;
        return 0;
    }

    if (href.size >= 7 && memcmp(href.text, "mailto:", 7) == 0) {
        link_type = mail_address;
        printf("\n.MT %.*s\n", href.size - 7, href.text + 7);
    } else {
        link_type = absolute_link;
        printf("\n.UR %.*s\n", href.size, href.text);
    }

    is_autolink = detail->is_autolink;

    if (is_autolink)
        push_text_mode(suppress_text);

    return 0;
}

int leave_link(void)
{
    if (link_type == relative_link)
        return 0;

    if (!is_autolink)
        printf("\n");

    if (link_type == mail_address)
        printf(".ME \\&");
    else
        printf(".UE \\&");

    if (is_autolink)
        pop_text_mode();

    return 0;
}

//=======//
// Image //
//=======//

int enter_image(void)
{
    warning("ignoring image element");
    push_text_mode(suppress_text);
    return 0;
}

int leave_image(void)
{
    pop_text_mode();
    return 0;
}
