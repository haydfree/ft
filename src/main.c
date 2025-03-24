#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client.h>

int main(void) {
    struct wl_display* display = wl_display_connect(NULL);

    if (display == NULL) {
        fprintf(stderr, "%s:%d Error: cannot establish wayland display connection", __FILE__, __LINE__);
        exit(1);
    }
     
    return 0;
}
