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
    int entryLen;
} TextContent;

typedef struct TextHistory {
    TextContent** arr;
    int size;
} TextHistory;

void onEnter(void);
void onEntry(char* entry);
void onBackSpace(void);
void render(AppContext* context);
void renderOutput(char* text);
TextContent* getCmd(void);
void clear(void);

#endif
