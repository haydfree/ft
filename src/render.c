#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "render.h"

static TextContent* content = NULL;
static TextHistory* history = NULL;

static TextContent*
initTextContent(void) {
    content = malloc(sizeof(TextContent));
    if (content == NULL) {
        fprintf(stderr, "%s:%d ERROR: content malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    content->text = malloc(sizeof(char) * MAX_TEXT_LEN);
    if (content->text == NULL) {
        fprintf(stderr, "%s:%d ERROR: content text malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    content->entry = malloc(sizeof(char) * MAX_ENTRY_LEN);
    if (content->entry == NULL) {
        fprintf(stderr, "%s:%d ERROR: content entry malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    memset(content->text, 0, MAX_TEXT_LEN);
    memset(content->entry, 0, MAX_ENTRY_LEN);
    content->textLen = 0;
    content->entryLen = 0;
    return content;
}

static TextHistory*
initTextHistory(void) {
    history = malloc(sizeof(TextHistory));
    if (history == NULL) {
        fprintf(stderr, "%s:%d ERROR: history malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    history->arr = malloc(sizeof(TextContent*) * NUM_ROWS);
    if (history->arr == NULL) {
        fprintf(stderr, "%s:%d ERROR: history arr malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    memset(history->arr, 0, NUM_ROWS*sizeof(TextContent*));
    history->size = 0;
    return history;
}

static void
resetEntry(void) {
    memset(content->entry, 0, MAX_ENTRY_LEN);
    content->entryLen = 0;
}

static void
resetText(void) {
    content = initTextContent();
}

static void
sendEntryToText(void) {
    if (content == NULL) {
        content = initTextContent();
    }
    strcat(content->text, content->entry);
    content->textLen += content->entryLen;
}

static void
sendTextToHistory(void) {
    if (history == NULL) {
        history = initTextHistory();
    }
    history->arr[history->size] = content;
    history->size += 1;
}

static void
removeLastFromText(void) {
    if (content == NULL) {
        content = initTextContent();
    }
    if (content->textLen <= 0) { return; }
    content->textLen -= 1;
    content->text[content->textLen] = '\0';
}

void
onEnter(void) {
    sendTextToHistory();
    resetText();
    resetEntry();
}

void
onBackSpace(void) {
    removeLastFromText();
    resetEntry();
}

void
onEntry(char* entry) {
    if (content == NULL) {
        content = initTextContent();
    }
    strncpy(content->entry, entry, MAX_ENTRY_LEN-1);
    content->entry[MAX_ENTRY_LEN-1]  ='\0';
    content->entryLen += 1;
    sendEntryToText();
    resetEntry();
}

void
render(AppContext* context) {
    int i, y;
    if (content == NULL) { initTextContent(); }
    if (history == NULL) { initTextHistory(); }
    if (history->size <= 0) { sendTextToHistory(); }
    XClearWindow(context->display, context->window);
    for (i = 0; i < history->size; i++) {
        y = 10 * i + 50; 
        XDrawString(context->display, context->window, context->gc, 50, y, history->arr[i]->text, 
            history->arr[i]->textLen);
    }
}
