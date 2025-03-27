#ifndef CONFIG_H
#define CONFIG_H

#include <X11/Xlib.h>
#include <window.h>

#define MAX_TEXT_LEN 1024
#define MAX_ENTRY_LEN 32

void initConfig(AppContext* context);

extern int windowX;
extern int windowY;
extern unsigned int windowWidth;
extern unsigned int windowHeight;
extern unsigned int borderWidth;
extern unsigned long borderColor;
extern unsigned long backgroundColor;
extern unsigned long foregroundColor;

extern size_t numColumns;
extern size_t numRows;
extern size_t textXOffset;
extern size_t textYOffset;

#endif

