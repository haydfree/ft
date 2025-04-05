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
    size_t bufSize;

    bufSize = 255;
    buf = malloc(sizeof(char) * bufSize);
    if (buf == NULL) {
        fprintf(stderr, "%s:%d ERROR: buf malloc failed\n", __FILE__, __LINE__);
        exit(1);
    }

    fp = popen(text, "r");
    fgets(buf, 255, fp);
    pclose(fp);

    return buf;
}

void
run(void) {
    AppContext* context = createContext();
    eventLoop(context);

    deleteContext(context);
}
