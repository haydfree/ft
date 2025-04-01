#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include "event.h"
#include "render.h"

void
eventLoop(AppContext* context) {
    XEvent event;
    KeySym key;
    char entry[MAX_ENTRY_LEN];
    int entryLen;
    int entryFlag;
    XSelectInput(context->display, context->window, KeyPressMask|ExposureMask|StructureNotifyMask);
    while (1) {
        render(context);
        XNextEvent(context->display, &event);
        entryLen = XLookupString(&event.xkey, entry, sizeof(entry)-1, &key, NULL);
        entryFlag = entryLen > 0;  
        switch (event.type) {
            case KeyPress:
                if (key == XK_Return) {
                    onEnter();
                    printf("%ld", key);
                } else if (key == XK_BackSpace) {
                    onBackSpace();
                } else if (entryFlag == 1) {
                    onEntry();
                }
                break;
            default:
                break;
        }
    }
}
