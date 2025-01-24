#include "types.h"
#include "ray.h"
#include <stdio.h>
#include <math.h>

#ifdef VISUALIZE
#include <raylib.h>
#endif

#define LIMIT(val, lim) ( (val > lim ? lim : val) )

// taken from the masters behind quake
float Q_rsqrt( float number ) {
    long i;
    float x2, y;

    x2 = number * .5f;
    y  = number;
    i  = * ( long * ) &y;						// evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( 1.5f - ( x2 * y * y ) );   // 1st iteration

    return y;
}

int get_height(float d, int max_h) {
    // float val = max_h*exp(-d*30/max_h);
    float val = max_h*20/d;
    return val;
}

float2 sum_f2(float2 v1, float2 v2) {
    return (float2) {v1.x + v2.x, v1.y + v2.y};
}

float2 scale_f2(float2 v, float c) {
    return (float2) {v.x*c, v.y*c};
}

float2 normalize_f2(float2 v) {
    return scale_f2(v, Q_rsqrt(v.x*v.x + v.y*v.y));
}


float check_collision(float2 origin, float angle, float distance_resolution, float max_distance, WallNode *head, Wall *c, float2 w2s) {
    float2 step = (float2){cosf(DEG2RAD(angle)), sinf(DEG2RAD(angle))};
    step = scale_f2(step, 1/distance_resolution);

    float2 point = origin;
    for (int i = 0; i < max_distance*distance_resolution; i++, point = sum_f2(point, step)) {
        for (WallNode *it = head; it; it = it->next) {
            Wall w = it->w;

            if (point.x > w.pos.x && point.x < w.pos.x+w.size.x && point.y > w.pos.y && point.y < w.pos.y+w.size.y) {
                *c = w;
                return sqrt((point.x - origin.x)*(point.x - origin.x) + (point.y - origin.y)*(point.y - origin.y));
            }

        }
    }

    return -1;
}

Color3 scale_colors(Color3 c1, int c) {
    return (Color3) {c1.r*c, c1.g*c, c1.b*c};
}

int cast(Frame f, Camera_ c, WallNode *head, float2 w2s) {
    float start_angle = c.dir_angle + (float)c.fov/2;
    float step_angle = (float) c.fov / c.n_rays;

    float angle = start_angle;
    int width = f.size.x / c.n_rays;

    Wall collision = {0};
    Color3 to_fill = {255, 0, 0};
    for (int r = 0; r < c.n_rays; r++, angle -= step_angle) {
        float d = check_collision(c.pos, angle, c.distance_resolution, c.max_distance, head, &collision, w2s);

        int h = get_height(d, f.size.y);

        to_fill = (Color3){0, 0, 0};

        if (d >= 0) {
            to_fill = collision.color;
        }

        for (int i = 0; i < f.size.y; i++) {
            for (int j = 0; j < width; j++) {
                if ( (i < f.size.y/2 + h/2 && i > f.size.y/2 - h/2)) {
                    float to_scale = d * 255/c.max_distance;
                    f.pixels[C(r*width+j, i, f.size.x)] = scale_colors(to_fill, LIMIT(to_scale, 255));
                }
                else 
                    f.pixels[C(r*width+j, i, f.size.x)] = (Color3) {0, 0, 0};
            }
        }
    }

    return 0;
}
