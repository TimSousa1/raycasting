#ifndef TIM_RAY_H
#define TIM_RAY_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "types.h"
#include "image.h"
#include "scene.h"


#define DEG2RAD(a) (a * M_PI/180)

typedef struct {
    float2 pos;
    float dir_angle;
    float2 vel;
} Object;

typedef struct {
    float2 pos;
    float dir_angle;
    int fov;
    int n_rays;
    float max_distance;
    float distance_resolution; // steps per unit
} Camera_;

typedef struct {
    Object obj;
    Camera_ cam;
} Entity;



// taken from the masters behind quake
float Q_rsqrt( float number );

// vector math
float2 sum_f2(float2 v1, float2 v2);
float2 scale_f2(float2 v, float c);
float2 normalize_f2(float2 v);

// collisions and general casting
#ifdef VISUALIZE
float check_collision(float2 origin, float angle, float distance_resolution, float max_distance, WallNode *head, Wall *collision, float2 world_to_screen_ratio);
int cast(Frame f, Camera_ c, WallNode *head, float2 world_to_screen_ratio);
#else
float check_collision(float2 origin, float angle, float distance_resolution, float max_distance, WallNode *head, Wall *collision);
int cast(Frame f, Camera_ c, WallNode *head);
#endif

#endif
