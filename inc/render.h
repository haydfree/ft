#ifndef RENDER_H
#define RENDER_H

#include "window.h"

#define MAX_TEXT_LEN 1024
#define MAX_ENTRY_LEN 32
#define NUM_ROWS 50
#define NUM_COLUMNS 50

typedef struct TextContent {
    char* text;
    char* entry;
    int textLen;
    size_t entryLen;
} TextContent;

typedef struct TextHistory {
    TextContent** arr;
    int size;
} TextHistory;

void onEnter(void);
void onEntry(void);
void onBackSpace(void);
void render(AppContext* context);

#endif
