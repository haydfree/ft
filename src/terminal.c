#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "terminal.h"
#include "window.h"
#include "event.h"

char*
runProcess(char* text) {
    FILE* fp;
    char* buf;
    size_t bufSize, ptr;

    bufSize = 255;
    ptr = 0;
    buf = malloc(sizeof(char) * bufSize);
    if (buf == NULL) {
        fprintf(stderr, "%s:%d ERROR: buf malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }
    memset(buf, 0, bufSize);

    fp = popen(text, "r");
    while (fgets(buf+ptr, bufSize-ptr, fp)) {
        ptr += strlen(buf+ptr); 
        if (ptr >= bufSize) { 
            bufSize *= 2;
            buf = realloc(buf, bufSize);
        }
    }
    pclose(fp);

    return buf;
}

void
run(void) {
    AppContext* context = createContext();
    eventLoop(context);

    deleteContext(context);
}
