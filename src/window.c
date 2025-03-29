#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include "window.h"

AppContext*
createContext() {
    AppContext* context;
    Display* display;
    int screen;
    Window window, parent;
    unsigned long black, white;
    GC gc;
    display = XOpenDisplay(NULL);    
    screen = DefaultScreen(display);
    parent = DefaultRootWindow(display);
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);
    window = XCreateSimpleWindow(display, parent, 50, 50, 500, 500, 1, white, black);
    gc = XCreateGC(display, window, 0, 0);
    XSetForeground(display, gc, white);
    XMapWindow(display, window);

    context = malloc(sizeof(AppContext));
    if (context == NULL) {
        fprintf(stderr, "%s:%d ERROR: context malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    context->display = display;
    context->screen = screen;
    context->window = window;
    context->parent = parent;
    context->gc = gc;
    return context;
}

void
deleteContext(AppContext* context) {
    XFreeGC(context->display, context->gc);
    XDestroyWindow(context->display, context->window);
    XCloseDisplay(context->display);
}
