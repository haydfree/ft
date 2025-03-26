#include <event.h>

int running = 1;

void
handleKeyPress(AppContext* context, TextContent* content, XEvent* event) {
    render(context, content, event);
}

void
handleExpose(AppContext* context, TextContent* content, XEvent* event) {
    render(context, content, event);
}

void
eventLoop(AppContext* context, TextContent* content) {
    XEvent event;
    XSelectInput(context->display, context->window, KeyPressMask | ExposureMask | StructureNotifyMask);
    while (running) {
        XNextEvent(context->display, &event);        

        switch (event.type) {
            case Expose:
                handleExpose(context, content, &event);
                break;
            case KeyPress:
                handleKeyPress(context, content, &event);
                break;
            default:
                break;
        }
    }
}
