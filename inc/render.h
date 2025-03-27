#ifndef RENDER_H
#define RENDER_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <window.h>

typedef struct TextContent {
    char* text;
    char* entry;
    size_t textLen;
    size_t entryLen;
} TextContent;

typedef struct TextHistory {
    TextContent** arr;
    size_t size;
} TextHistory;

void render(AppContext* context, XEvent* event);

#endif
