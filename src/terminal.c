#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "terminal.h"
#include "window.h"
#include "event.h"

char*
runProcess(char* text) {
    FILE* fp;
    char* buf;
    int bufSize;

    buf = malloc(sizeof(char) * bufSize);
    if (buf == NULL) {
        fprintf(stderr, "%s:%d ERROR: buf malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    bufSize = 255;
    fp = popen(text);
    fgets(buf, bufSize, fp);
    pclose(fp);

    return buf;
}

void
run(void) {
    AppContext* context = createContext();
    eventLoop(context);

    deleteContext(context);
}
