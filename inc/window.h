#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>

typedef struct AppContext {
    Display* display;
    int screen;
    Window window;
    Window parent;
    GC gc;
} AppContext;

AppContext* createContext(void);
void deleteContext(AppContext* context);

#endif
