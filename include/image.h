#ifndef TIM_IMAGE_H
#define TIM_IMAGE_H

#include "types.h"


#define C(x, y, X) ((y)*(X) + (x))
#define is_in_bounds(x, y, maxx, maxy) (x > 0 && x < maxx && y > 0 && y < maxy)


typedef struct {
    int2 size;
    Color3 *pixels;
} Frame;

int write_image(Frame f, char *out_file);
void print_frame(Frame f);

// NOT MEMORY SAFE! CAREFUL WITH out_image AND sizex and sizey
// TURNS TO GRAYSCALE FOR NOW.. WHOOPS
int downscale(Color3 *out_image, Frame f, uint sizex, uint sizey);
int color_region(Color3 *image, Color3 color, int2 pos, int2 size, int xmax, int ymax);

#endif
