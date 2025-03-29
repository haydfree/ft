#include "terminal.h"
#include "window.h"
#include "event.h"

void
run(void) {
    AppContext* context = createContext();
    eventLoop(context);

    deleteContext(context);
}
