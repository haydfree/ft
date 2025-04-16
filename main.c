#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 12
#define START_X 10
#define START_Y 20
#define MAX_CHARS 1024
#define PROMPT "> "

Display* dpy;
Window win;
GC gc;
int screen;

char text[MAX_CHARS];
size_t textLen, cursorPos, cx, cy;

void
initDisplay()
{
    dpy = XOpenDisplay(NULL);
    screen = DefaultScreen(dpy);
    win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 0, 0, 800, 600, 1,
                               WhitePixel(dpy, screen), BlackPixel(dpy, screen));
    XSelectInput(dpy, win, ExposureMask | KeyPressMask);
    XMapWindow(dpy, win);
    gc = XCreateGC(dpy, win, 0, NULL);
    XSetForeground(dpy, gc, WhitePixel(dpy, screen));
}

void
drawChar(char c, size_t x, size_t y)
{
    char str[2];
    str[0] = c;
    str[1] = '\0';
    XDrawString(dpy, win, gc, (int)x, (int)y, str, 1);
}

void
clearChar(size_t x, size_t y)
{
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XFillRectangle(dpy, win, gc, (int)x, (int)y - CHAR_HEIGHT + 2, CHAR_WIDTH, CHAR_HEIGHT);
    XSetForeground(dpy, gc, WhitePixel(dpy, screen));
}

void
drawCursor()
{
    XFillRectangle(dpy, win, gc, (int)cx, (int)cy - CHAR_HEIGHT + 2, CHAR_WIDTH, CHAR_HEIGHT);
}

void
clearCursor()
{
    clearChar(cx, cy);
}

void
drawPrompt()
{
    size_t i;
    for (i = 0; PROMPT[i] != '\0'; i++) {
        drawChar(PROMPT[i], START_X + i * CHAR_WIDTH, cy);
    }
}

void
redrawText()
{
    size_t i;
    size_t x = START_X + strlen(PROMPT) * CHAR_WIDTH;
    drawPrompt();
    for (i = 0; i < textLen; i++) {
        drawChar(text[i], x + i * CHAR_WIDTH, cy);
    }
    cx = START_X + strlen(PROMPT) * CHAR_WIDTH + cursorPos * CHAR_WIDTH;
}

void
moveCursorLeft()
{
    if (cursorPos > 0) {
        clearCursor();
        cursorPos--;
        cx -= CHAR_WIDTH;
    }
}

void
moveCursorRight()
{
    if (cursorPos < textLen) {
        clearCursor();
        cursorPos++;
        cx += CHAR_WIDTH;
    }
}

void
insertChar(char c)
{
    size_t i;
    if (textLen >= MAX_CHARS) {
        return;
    }

    for (i = textLen; i > cursorPos; i--) {
        text[i] = text[i - 1];
    }

    text[cursorPos] = c;
    textLen++;
    cursorPos++;
    cx += CHAR_WIDTH;
}

void
deleteChar()
{
    size_t i;
    if (cursorPos == 0 || textLen == 0) {
        return;
    }

    for (i = cursorPos - 1; i < textLen - 1; i++) {
        text[i] = text[i + 1];
    }

    textLen--;
    cursorPos--;
    cx -= CHAR_WIDTH;
}

void
handleReturn()
{
    memset(text, 0, MAX_CHARS);
    textLen = 0;
    cursorPos = 0;
    cx = START_X + strlen(PROMPT) * CHAR_WIDTH;
    cy += CHAR_HEIGHT;
}

void
handleKeyPress(XEvent* ev)
{
    KeySym keySym;
    char buf[32];
    int len;
    size_t i;

    len = XLookupString(&ev->xkey, buf, sizeof(buf), &keySym, NULL);

    clearCursor();

    if (keySym == XK_Left) {
        moveCursorLeft();
    } else if (keySym == XK_Right) {
        moveCursorRight();
    } else if (keySym == XK_BackSpace) {
        deleteChar();
    } else if (keySym == XK_Return) {
        handleReturn();
    } else if (len == 1) {
        insertChar(buf[0]);
    }

    for (i = 0; i < 800 / CHAR_WIDTH; i++) {
        clearChar(START_X + i * CHAR_WIDTH, cy);
    }

    redrawText();
    drawCursor();
}

void
eventLoop()
{
    XEvent e;
    while (1) {
        XNextEvent(dpy, &e);
        if (e.type == Expose) {
            redrawText();
            drawCursor();
        } else if (e.type == KeyPress) {
            handleKeyPress(&e);
        }
    }
}

int
main()
{
    textLen = 0;
    cursorPos = 0;
    cx = START_X + strlen(PROMPT) * CHAR_WIDTH;
    cy = START_Y;
    initDisplay();
    eventLoop();
    return 0;
}

