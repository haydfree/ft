#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include "event.h"
#include "render.h"
#include "terminal.h"

void
eventLoop(AppContext* context) {
    XEvent event;
    KeySym key;
    char entry[MAX_ENTRY_LEN];
    int entryLen;
    int entryFlag;
    TextContent* content;

    XSelectInput(context->display, context->window, KeyPressMask|ExposureMask|StructureNotifyMask);
    while (1) {
        memset(&event, 0, sizeof(XEvent));
        memset(&entry, 0, sizeof(entry));
        key = 0;
        entryLen = 0;
        entryFlag = 0;
        render(context);
        XNextEvent(context->display, &event);
        entryLen = XLookupString(&event.xkey, entry, sizeof(entry)-1, &key, NULL);
        entryFlag = entryLen > 0;  
        switch (event.type) {
            case KeyPress:
                if (key == XK_Return) {
                    onEnter();
                    content = getCmd();
                    renderOutput(runProcess(content->text));
                } else if (key == XK_BackSpace) {
                    onBackSpace();
                } else if (entryFlag == 1) {
                    onEntry(entry);
                }
                break;
            default:
                break;
        }
    }
}
