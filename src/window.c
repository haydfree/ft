#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <window.h>
#include <config.h>

static Display* 
openDisplay(void) {
    Display* display = XOpenDisplay(NULL);
    assert(display != NULL);
    return display;
}

static int 
getScreen(Display* display) {
    assert(display != NULL);
    return DefaultScreen(display);
}

static Window 
getParentWindow(Display* display, int screen) {
    assert(display != NULL && screen >= 0);
    return RootWindow(display, screen);
}

static Window
openWindow(Display* display, Window parent, int x, int y, unsigned int width, unsigned int height,
    unsigned int borderWidth, unsigned long borderColor, unsigned long backgroundColor) {
    assert(display != NULL && x >= 0 && y >= 0);
    return XCreateSimpleWindow(display, parent, x, y, width, height, borderWidth, borderColor, backgroundColor);
}

static GC
createGC(Display* display, Window window) {
    GC gc;
    assert(display != NULL);
    gc = XCreateGC(display, window, 0, NULL);
    XSetForeground(display, gc, borderColor);
    return gc;
}

AppContext* initAppContext(void) {
    Display* display;
    int screen;
    Window parent, window;
    GC gc;
    AppContext* context;

    display = openDisplay(); 
    screen = getScreen(display);
    context = malloc(sizeof(AppContext));
    assert(context != NULL);
    context->display = display;
    context->screen = screen;
    initConfig(context);
    parent = getParentWindow(display, screen);
    window = openWindow(display, parent, windowX, windowY, windowWidth, windowHeight, 
        borderWidth, borderColor, backgroundColor);
    gc = createGC(display, window);
    context->window = window;
    context->parent = parent;
    context->gc = gc;

    return context;
}

void
mapWindow(AppContext* context) {
    assert(context->display != NULL);
    XMapWindow(context->display, context->window);
}

void
closeDisplay(AppContext* context) {
    XFreeGC(context->display, context->gc);
    XCloseDisplay(context->display);
    free(context);
}

