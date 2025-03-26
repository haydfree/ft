#include <event.h>
#include <render.h>

int running = 1;

void
handleKeyPress(AppContext* context, XEvent* event) {
    drawText(context, event);
}

void
handleExpose(AppContext* context, XEvent* event) {
    (void) context;
    (void) event;
}

void
eventLoop(AppContext* context) {
    XEvent event;
    XSelectInput(context->display, context->window, KeyPressMask | ExposureMask | StructureNotifyMask);
    while (running) {
        XNextEvent(context->display, &event);        

        switch (event.type) {
            case Expose:
                handleExpose(context, &event);
                break;
            case KeyPress:
                handleKeyPress(context, &event);
                break;
            default:
                break;
        }
    }
}
