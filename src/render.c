#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <render.h>
#include <config.h>
#include <assert.h>

static TextContent* content = NULL;
static TextHistory* history = NULL;

static TextHistory*
initTextHistory() {
    if (history != NULL) { return history; }
    history = malloc(sizeof(TextHistory));
    assert(history != NULL);
    history->arr = malloc(sizeof(TextContent*) * numRows);
    assert(history->arr != NULL);
    history->size = 0;
    return history;
}

static TextContent*
initTextContent() {
    content = malloc(sizeof(TextContent));
    assert(content != NULL);
    content->text = malloc(sizeof(char) * MAX_TEXT_LEN);
    content->entry = malloc(sizeof(char) * MAX_ENTRY_LEN);
    content->textLen = 0;
    content->entryLen = 0;
    memset(content->text, 0, MAX_TEXT_LEN);
    memset(content->entry, 0, MAX_ENTRY_LEN);
    return content;
}

static void
refreshWindow(AppContext* context) {
    size_t i;
    XClearWindow(context->display, context->window);
    
    for (i = 0; i < history->size; i++) {
        XDrawString(context->display, context->window, context->gc, 
            textXOffset, textYOffset+(windowHeight/numRows)*i,
            history->arr[i]->text, history->arr[i]->textLen);
    }
}

static void
incrementHistorySize(TextHistory* history) {
    history->size += 1;
}

static void
putTextToHistory(TextContent* content, TextHistory* history) {
    history->arr[history->size-1] = content;
}

static void
resetText() {
    content = initTextContent();
}

static void
putEntryToText(TextContent* content) {
    content->entry[content->entryLen] = '\0';
    strcat(content->text, content->entry);
    content->textLen += content->entryLen;
}

static void
resetEntry(TextContent* content) {
    memset(content->entry, 0, MAX_ENTRY_LEN);
    content->entryLen = 0;
}

static void
removeOneFromText(TextContent* content) {
    if (content->textLen <= 0) { return; }
    content->textLen -= 1;
    content->text[content->textLen] = '\0';
}

static int
isTextInHistory(TextContent* content, TextHistory* history) {
    
}

void
render(AppContext* context, XEvent* event) {
    KeySym keysym;

    if (content == NULL) { initTextContent(); }
    if (history == NULL) { initTextHistory(); }

    content->entryLen = XLookupString(&event->xkey, content->entry,
        sizeof(content->entry)-1, &keysym, NULL);

    if (keysym == XK_Return) {
        putEntryToText(content);
        resetText();
    } else if (keysym == XK_BackSpace) {
        removeOneFromText(content);
        resetEntry(content);
    } else if (content->entryLen > 0 && 
        content->textLen+content->entryLen < MAX_TEXT_LEN-1) {
        if (content->textLen == 0) {
            putEntryToText(content);
            incrementHistorySize(history);
            putTextToHistory(content, history);
            resetEntry(content);
        } else if (content->textLen > 0) {
            putEntryToText(content);
            resetEntry(content);
        }
    } 

    refreshWindow(context);
}


