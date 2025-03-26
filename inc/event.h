#ifndef EVENT_H
#define EVENT_H

#include <X11/Xlib.h>
#include <window.h>
#include <render.h>

void handleKeyPress(AppContext* context, TextContent* content, XEvent* event);
void handleExpose(AppContext* context, TextContent* content, XEvent* event);
void eventLoop(AppContext* context, TextContent* content);

#endif
