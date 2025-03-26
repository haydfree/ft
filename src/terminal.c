#include <terminal.h>
#include <window.h>
#include <event.h>
#include <config.h>

void
run(void) {
    AppContext* context;
    context = initAppContext();

    eventLoop(context);

}
