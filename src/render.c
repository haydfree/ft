#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <render.h>
#include <config.h>
#include <assert.h>

static void
refreshWindow(AppContext* context, TextContent* content) {
    XClearWindow(context->display, context->window);
    XDrawString(context->display, context->window, context->gc, 10, 50, content->text, content->textLen);
}

TextContent*
initTextContent() {
    TextContent* content = malloc(sizeof(TextContent));
    assert(content != NULL);

    content->text = malloc(sizeof(char) * MAX_TEXT_LEN);
    content->entry = malloc(sizeof(char) * MAX_ENTRY_LEN);
    content->textLen = 0;
    content->entryLen = 0;
    memset(content->text, 0, MAX_TEXT_LEN);
    memset(content->entry, 0, MAX_ENTRY_LEN);

    return content;
}

void
render(AppContext* context, TextContent* content, XEvent* event) {
    KeySym keysym;

    content->entryLen = XLookupString(&event->xkey, content->entry, sizeof(content->entry)-1, &keysym, NULL);

    if (keysym == XK_Return) {
        content->textLen = 0;
        content->text[0] = '\0';
    } else if (keysym == XK_BackSpace) {
        if (content->textLen > 0) {
            content->textLen--;
            content->text[content->textLen] = '\0';
        }
    } else if (content->entryLen > 0 && content->textLen+content->entryLen < MAX_TEXT_LEN-1) {
        content->entry[content->entryLen] = '\0';
        strcat(content->text, content->entry);
        content->textLen += content->entryLen;
    } 

    refreshWindow(context, content);
}


