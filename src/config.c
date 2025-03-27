#include <config.h>

int windowX = 0;
int windowY = 0;
unsigned int windowWidth = 500;
unsigned int windowHeight = 300;
unsigned int borderWidth = 10;
unsigned long borderColor;
unsigned long backgroundColor;
unsigned long foregroundColor;

size_t numColumns = 50;
size_t numRows = 20;
size_t textXOffset;
size_t textYOffset;

void initConfig(AppContext* context) {
    borderColor = WhitePixel(context->display, context->screen);
    backgroundColor = BlackPixel(context->display, context->screen);
    foregroundColor = WhitePixel(context->display, context->screen);

    textXOffset = windowX + 50;
    textYOffset = windowY + 50;
}

