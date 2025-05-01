#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <safecast/safecast.h>

#define FT_CHAR_WIDTH 6
#define FT_CHAR_HEIGHT 12
#define FT_START_X 10
#define FT_START_Y 20
#define FT_MAX_CHARS 1024
#define FT_PROMPT "$ "

Display* dpy;
Window win;
GC gc;
int screen;
char text[FT_MAX_CHARS];
size_t textLen, cursorPos, cx, cy;

void
initDisplay(void)
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
    int sx, sy;
    str[0] = c;
    str[1] = '\0';
    if (safeCastToInt_sizet(x, &sx) && safeCastToInt_sizet(y, &sy))
    {
        XDrawString(dpy, win, gc, sx, sy, str, 1);
    } else
    {
        fprintf(stderr, "%s:%d ERROR: could not safely cast\n", __FILE__, __LINE__);
        exit(1);
    }
}

void
clearChar(size_t x, size_t y)
{
    int sx, sy;
    if (safeCastToInt_sizet(x, &sx) && safeCastToInt_sizet(y, &sy))
    {
        XSetForeground(dpy, gc, BlackPixel(dpy, screen));
        XFillRectangle(dpy, win, gc, sx, sy-FT_CHAR_HEIGHT+2, FT_CHAR_WIDTH, FT_CHAR_HEIGHT);
        XSetForeground(dpy, gc, WhitePixel(dpy, screen));
    } else 
    {
        fprintf(stderr, "%s:%d ERROR: could not safely cast\n", __FILE__, __LINE__);
        exit(1);
    }
}

void
drawCursor(void)
{
    int scx, scy;
    if (safeCastToInt_sizet(cx, &scx) && safeCastToInt_sizet(cy, &scy))
    {
        XFillRectangle(dpy, win, gc, scx, scy-FT_CHAR_HEIGHT+2, FT_CHAR_WIDTH, FT_CHAR_HEIGHT);
    } else 
    {
        fprintf(stderr, "%s:%d ERROR: could not safely cast\n", __FILE__, __LINE__);
        exit(1);
    }
}

void
clearCursor(void)
{
    clearChar(cx, cy);
}

void
drawPrompt(void)
{
    size_t i;
    for (i = 0; FT_PROMPT[i] != '\0'; i++) 
    {
        drawChar(FT_PROMPT[i], FT_START_X + i * FT_CHAR_WIDTH, cy);
    }
}

void
drawText(void)
{
    size_t i;
    size_t x;

    x = FT_START_X + strlen(FT_PROMPT) * FT_CHAR_WIDTH;
    drawPrompt();
    for (i = 0; i < textLen; i++) 
    {
        drawChar(text[i], x + i * FT_CHAR_WIDTH, cy);
    }
    cx = FT_START_X + strlen(FT_PROMPT) * FT_CHAR_WIDTH + cursorPos * FT_CHAR_WIDTH;
}

void
clearText(void)
{
    size_t i;
    for (i = 0; i < 800 / FT_CHAR_WIDTH; i++) 
    {
        clearChar(FT_START_X + i * FT_CHAR_WIDTH, cy);
    }
}

void
insertChar(char c)
{
    size_t i;
    if (textLen >= FT_MAX_CHARS) 
    {
        return;
    }

    for (i = textLen; i > cursorPos; i--) 
    {
        text[i] = text[i - 1];
    }

    text[cursorPos] = c;
    textLen++;
    cursorPos++;
    cx += FT_CHAR_WIDTH;
}

void
deleteChar(void)
{
    size_t i;
    if (cursorPos == 0 || textLen == 0) 
    {
        return;
    }

    for (i = cursorPos - 1; i < textLen - 1; i++) 
    {
        text[i] = text[i + 1];
    }

    textLen--;
    cursorPos--;
    cx -= FT_CHAR_WIDTH;
}

void
moveCursorLeft(void)
{
    if (cursorPos > 0) 
    {
        clearCursor();
        cursorPos--;
        cx -= FT_CHAR_WIDTH;
    }
}

void
moveCursorRight(void)
{
    if (cursorPos < textLen) 
    {
        clearCursor();
        cursorPos++;
        cx += FT_CHAR_WIDTH;
    }
}

void
parseInput(void)
{
}

void
handleReturn(void)
{
    memset(text, 0, FT_MAX_CHARS);
    textLen = 0;
    cursorPos = 0;
    cx = FT_START_X + strlen(FT_PROMPT) * FT_CHAR_WIDTH;
    cy += FT_CHAR_HEIGHT;
}

void
handleKeyPress(XEvent* ev)
{
    KeySym keySym;
    char buf[32];
    int len;

    len = XLookupString(&ev->xkey, buf, sizeof(buf), &keySym, NULL);
    clearCursor();

    if (keySym == XK_Left) 
    {
        moveCursorLeft();
    } else if (keySym == XK_Right) 
    {
        moveCursorRight();
    } else if (keySym == XK_BackSpace) 
    {
        deleteChar();
    } else if (keySym == XK_Return) 
    {
        handleReturn();
    } else if (len == 1) 
    {
        insertChar(buf[0]);
    }

    clearText();
    drawText();
    drawCursor();
}

void
eventLoop(void)
{
    XEvent e;
    while (1) 
    {
        XNextEvent(dpy, &e);
        if (e.type == Expose) 
        {
            drawText();
            drawCursor();
        } else if (e.type == KeyPress) 
        {
            handleKeyPress(&e);
        }
    }
}

int
main(void)
{
    textLen = 0;
    cursorPos = 0;
    cx = FT_START_X + strlen(FT_PROMPT) * FT_CHAR_WIDTH;
    cy = FT_START_Y;
    initDisplay();
    eventLoop();
    return 0;
}

