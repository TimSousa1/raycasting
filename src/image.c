#include <stdio.h>
#include "image.h"

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
