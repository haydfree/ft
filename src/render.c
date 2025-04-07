#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "render.h"

static TextContent* content = NULL;
static TextHistory* history = NULL;

static TextContent*
initTextContent(void) {
    TextContent* c;

    c = malloc(sizeof(TextContent));
    if (c == NULL) {
        fprintf(stderr, "%s:%d ERROR: content malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    c->text = malloc(sizeof(char) * MAX_TEXT_LEN);
    if (c->text == NULL) {
        fprintf(stderr, "%s:%d ERROR: content text malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    c->entry = malloc(sizeof(char) * MAX_ENTRY_LEN);
    if (c->entry == NULL) {
        fprintf(stderr, "%s:%d ERROR: content entry malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    memset(c->text, 0, MAX_TEXT_LEN);
    memset(c->entry, 0, MAX_ENTRY_LEN);
    c->textLen = 0;
    c->entryLen = 0;
    return c;
}

static TextHistory*
initTextHistory(void) {
    TextHistory* h;

    h = malloc(sizeof(TextHistory));
    if (h == NULL) {
        fprintf(stderr, "%s:%d ERROR: history malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    h->arr = malloc(sizeof(TextContent*) * NUM_ROWS);
    if (h->arr == NULL) {
        fprintf(stderr, "%s:%d ERROR: history arr malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    memset(h->arr, 0, NUM_ROWS*sizeof(TextContent*));
    h->size = 0;
    return h;
}

static void
resetEntry(void) {
    memset(content->entry, 0, MAX_ENTRY_LEN);
    content->entryLen = 0;
}

static void
resetText(void) {
    memset(content->text, 0, MAX_TEXT_LEN);
    memset(content->entry, 0, MAX_ENTRY_LEN);
    content->textLen = 0;
    content->entryLen = 0;
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
    TextContent* c;
    if (history == NULL) {
        history = initTextHistory();
    }
    if (content == NULL) {
        fprintf(stderr, "%s:%d ERROR: sending null content to history", __FILE__, __LINE__);
        exit(1);
    }
    c = initTextContent(); 
    strncpy(c->text, content->text, MAX_TEXT_LEN-1);
    c->text[MAX_TEXT_LEN-1] = '\0';
    c->textLen = content->textLen;
    history->arr[history->size] = c;
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

static void
drawShellPrompt(AppContext* context, int x, int y) {
    char prompt[] = {(char)36, '\0'};
    XDrawString(context->display, context->window, context->gc, x, y, prompt, 1);
}

static void
drawShellCursor(AppContext* context, int x, int y) {
    char cursor[] = {(char)95, '\0'};
    XDrawString(context->display, context->window, context->gc, x, y, cursor, 1);
}

TextContent*
getCmd(void) {
    return history->arr[history->size-1];
}

void
renderOutput(char* text) {
    TextContent* c;
    if (history == NULL) {
        history = initTextHistory();
    }
    if (text == NULL) {
        fprintf(stderr, "%s:%d ERROR: sending null output to history", __FILE__, __LINE__);
        exit(1);
    }
    c = initTextContent(); 
    strncpy(c->text, text, MAX_TEXT_LEN-1);
    c->textLen = strlen(text);
    history->arr[history->size] = c;
    history->size += 1;
}

void
render(AppContext* context) {
    int i, y;
    if (content == NULL) { content = initTextContent(); }
    if (history == NULL) { history = initTextHistory(); }
    XClearWindow(context->display, context->window);
    for (i = 0; i < history->size; i++) {
        y = 15 * i + 50; 
        drawShellPrompt(context, 50-15, y);
        XDrawString(context->display, context->window, context->gc, 50, y, history->arr[i]->text, 
            history->arr[i]->textLen);
    }
    for (i = 0; i < 1; i++) {
        y = history->size * 15 + 50;
        drawShellPrompt(context, 50-15, y);
        XDrawString(context->display, context->window, context->gc, 50, y, content->text, content->textLen);
        drawShellCursor(context, 50+content->textLen*6, y);
    }
}
