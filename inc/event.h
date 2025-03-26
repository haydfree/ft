#ifndef EVENT_H
#define EVENT_H

#include <X11/Xlib.h>
#include <window.h>

void handleKeyPress(AppContext* context, XEvent* event);
void handleExpose(AppContext* context, XEvent* event);
void eventLoop(AppContext* context);

#endif
