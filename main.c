#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <X11/Xlib.h>

uint64_t scn, wdx, wdy, cx, cy, wth, hht, bwh, bdr, bgd; 
Display* dpy;
Window wdw, pnt;
GC gc;

int
convInt(uint64_t i) {
}

void
initDpy(void) {
    dpy = XOpenDisplay(NULL);
    scn = DefaultScreen(dpy);
    assert(scn <= INT16_MAX);
    pnt = RootWindow(dpy, (int) scn);
    wdx = 10;
    wdy = 10;
    wth = 800;
    hht = 600;
    bwh = 1;
    bdr = WhitePixel(dpy, (int) scn);
    bgd = BlackPixel(dpy, (int) scn);
    wdw = XCreateSimpleWindow(dpy, pnt, wdx, wdy, wth, hht, bwh, bdr, bgd);
    XSelectInput(dpy, wdw, ExposureMask | KeyPressMask);
    XMapWindow(dpy, wdw);
    gc = XCreateGC(dpy, wdw, 0, NULL);
    XSetForeground(dpy, gc, WhitePixel(dpy, scn));
}

void
drawInput(char c) {
    char str[2];
    if (cx == 0) { cx = 10; }
    if (cy == 0) { cy = 10; }
    str[0] = c;
    str[1] = '\0';
    XDrawString(dpy, wdw, gc, cx, cy, str, 1);
    cx+=6;
    if (cx >= 0 && (unsigned int) cx >= wth) { cx = 10; cy+=10; }
    XFlush(dpy);
}

char
getInput(void) {
    XEvent e;
    KeySym key;
    XNextEvent(dpy, &e);    
    if (e.type == KeyPress) {
        key = XLookupKeysym(&e.xkey, 0);
        if (key <= 127) {
            return (char) key;
        }
    }
    return '\0';
}

void
eventLoop(void) {
    while (1) {
        char c;
        c = getInput();
        drawInput(c);
    }
}

void
closeDpy(void) {
    XDestroyWindow(dpy, wdw);
    XCloseDisplay(dpy);
}

int
main(void) {
    initDpy();
    eventLoop();
    closeDpy();
    return 0;
}
