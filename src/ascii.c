#include "types.h"
#include "image.h"
#include <stdio.h>


char *ascii(char *frame_ascii, Frame f) {
    char bright_ascii[] = " .,<=#%iHM";
    int n_bright_levels = sizeof(bright_ascii) / sizeof(*bright_ascii);

    int i = 0;
    for (int y = 0; y < f.size.y; y++) {
        for (int x = 0; x < f.size.x; x++) {
            int p = C(x, y, f.size.x);
            int brightness = (f.pixels[p].r + f.pixels[p].g + f.pixels[p].b) / 3;
            frame_ascii[i] = bright_ascii[brightness * n_bright_levels / 255];
            i++;
        }
        frame_ascii[i] = '\n';
        i++;
    }

    return frame_ascii;
}

int print_to_term(char *frame_ascii) {
    while(*frame_ascii) {
        putc(*frame_ascii, stdout);
        if (*frame_ascii != '\n') 
            putc(*frame_ascii, stdout);

        frame_ascii++;
    }
    return 0;
}
