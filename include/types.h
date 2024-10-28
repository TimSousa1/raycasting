#ifndef TIM_TYPES_H
#define TIM_TYPES_H

typedef unsigned char uchar;

typedef struct {
    int x;
    int y;
} int2;

typedef struct {
    float x;
    float y;
} float2;

typedef struct {
    uchar r;
    uchar g;
    uchar b;
    uchar a;
} Color3;

#endif
