#ifndef RENDER_H
#define RENDER_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <window.h>

void drawText(AppContext* context, XEvent* event);

#endif
