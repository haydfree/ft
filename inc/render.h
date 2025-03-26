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

TextContent* initTextContent();
void render(AppContext* context, TextContent* content, XEvent* event);

#endif
