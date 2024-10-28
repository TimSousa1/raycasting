#ifndef TIM_IMAGE_H
#define TIM_IMAGE_H

#include "types.h"


#define C(x, y, X) (y*X + x)



typedef struct {
    int2 size;
    Color3 *pixels;
} Frame;

int write_image(Frame f, char *out_file);
void print_frame(Frame f);

#endif
