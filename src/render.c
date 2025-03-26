#include <X11/Xutil.h>
#include <string.h>
#include <render.h>
#include <config.h>

static void
refreshWindow(AppContext* context, char* text, size_t textLen) {
    XClearWindow(context->display, context->window);
    XDrawString(context->display, context->window, context->gc, 10, 50, text, textLen);
}

void
drawText(AppContext* context, XEvent* event) {
    char text[MAX_TEXT_LEN];    
    char entry[MAX_ENTRY_LEN];
    size_t textLen;
    size_t entryLen;
    KeySym keysym;

    memset(text, 0, sizeof(text));
    memset(entry, 0, sizeof(entry));
    textLen = 0;
    entryLen = XLookupString(&event->xkey, entry, sizeof(entry)-1, &keysym, NULL);

    if (keysym == XK_Return) {
        textLen = 0;
        text[0] = '\0';
    } else if (keysym == XK_BackSpace) {
        if (textLen > 0) {
            textLen--;
            text[textLen] = '\0';
        }
    } else if (entryLen > 0 && textLen+entryLen < MAX_TEXT_LEN-1) {
        entry[entryLen] = '\0';
        strcat(text, entry);
        textLen += entryLen;
    } 

    refreshWindow(context, text, textLen);
}


