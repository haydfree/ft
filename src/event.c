#include <event.h>

int running = 1;

static void
handleKeyPress(AppContext* context, XEvent* event) {
    render(context, event);
}

void
eventLoop(AppContext* context) {
    XEvent event;
    XSelectInput(context->display, context->window, KeyPressMask | ExposureMask | StructureNotifyMask);
    while (running) {
        XNextEvent(context->display, &event);        

        switch (event.type) {
            case KeyPress:
                handleKeyPress(context, &event);
                break;
            default:
                break;
        }
    }
}
