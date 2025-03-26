#include <terminal.h>
#include <window.h>
#include <event.h>
#include <config.h>

void
run(void) {
    AppContext* context;
    TextContent* content;

    context = initAppContext();
    content = initTextContent(context);
    mapWindow(context);

    eventLoop(context, content);
}
