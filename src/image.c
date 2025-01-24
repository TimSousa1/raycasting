#include <stdio.h>
#include "image.h"

#define c3i(b) (Color3) {b, b, b}

int color_region(Color3 *image, Color3 color, int2 pos, int2 size, int xmax, int ymax) {
    for (int y = pos.y; y < pos.y+size.y; y++) {
        for (int x = pos.x; x < pos.x+size.x; x++) {
            if (!is_in_bounds(x, y, xmax, ymax)) continue;
            int i = C(x, y, xmax);
            image[i] = color;
        }
    }
    return 0;
}

// NOT MEMORY SAFE! CAREFUL WITH out_image AND sizex and sizey
// TURNS TO GRAYSCALE FOR NOW.. WHOOPS
int downscale(Color3 *out_image, Frame f, uint sizex, uint sizey) {
    int brightness = 0;
    int p = 0;
    int ratiox = f.size.x/sizex;
    int ratioy = f.size.y/sizey;

    // printf("image ratio: %d, %d\n", ratiox, ratioy);
    int i = 0; // indexing output image
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

            int avg = brightness / (ratiox*ratioy);
            out_image[i] = c3i(avg);
            brightness = 0;
            i++;
        }
    }
    return 0;
}

int write_image(Frame f, char *out_file){
    FILE *rendered_image = fopen(out_file, "w");
    if (!rendered_image) {
        fprintf(stderr, "Couldn't open file for saving!\n");
        return 1;
    }

    fprintf(rendered_image, "P3 %i %i %i\n", f.size.x, f.size.y, 255);

    for (int y = 0; y < f.size.y; y++){
        for (int x = 0; x < f.size.x; x++){
            fprintf(rendered_image, "%hhu %hhu %hhu ", f.pixels[C(x, y, f.size.x)].r,
                                                       f.pixels[C(x, y, f.size.x)].g,
                                                       f.pixels[C(x, y, f.size.x)].b);
        } 
        fprintf(rendered_image, "\n");
    }
    fclose(rendered_image);
    return 0;
}

void print_frame(Frame f) {
    for (int i = 0; i < f.size.x; i++) {
        for (int j = 0; j < f.size.y; j++) {
            Color3 cur = f.pixels[C(i, j, f.size.x)];
            printf("%hhu %hhu %hhu\t", cur.r, cur.g, cur.b);
        }
        printf("\n");
    }
}
