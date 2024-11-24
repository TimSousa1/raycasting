#include "types.h"
#include "image.h"

#include <stdlib.h>


char *ascii(char *frame_ascii, size_t sizex, size_t sizey, Frame f) {
    char bright_ascii[] = ".,<=#%iHM";
    int n_bright_levels = sizeof(bright_ascii) / sizeof(*bright_ascii);

    int brightness = 0;
    int p = 0;
    int ratiox = f.size.x/sizex;
    int ratioy = f.size.y/sizey;

    int i = 0;
    for (int y = 0; y < f.size.y; y+=ratioy) {
        for (int x = 0; x < f.size.x; x+=ratiox) {

            for (int yy = 0; yy < ratioy; yy++) {
                for (int xx = 0; xx < ratiox; xx++) {
                    p = C(x+xx, y+yy, f.size.x);

                    int b = 0;
                    b += f.pixels[p].r;
                    b += f.pixels[p].g;
                    b += f.pixels[p].b;
                    b /= 3;

                    brightness += b;
                }
            }

            frame_ascii[i] = bright_ascii[brightness/(ratiox*ratioy) * n_bright_levels / 255];
            brightness = 0;
            i++;
        }
        frame_ascii[i] = '\n';
        i++;
    }

    return frame_ascii;
}
