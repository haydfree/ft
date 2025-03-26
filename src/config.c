#include <config.h>

int windowX = 0;
int windowY = 0;
unsigned int windowWidth = 500;
unsigned int windowHeight = 300;
unsigned int borderWidth = 10;
unsigned long borderColor;
unsigned long backgroundColor;
unsigned long foregroundColor;

void initConfig(Display* display, int screen) {
    borderColor = WhitePixel(display, screen);
    backgroundColor = BlackPixel(display, screen);
    foregroundColor = WhitePixel(display, screen);
}

