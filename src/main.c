#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define C(x, y, X) (y*X + x)

#define DEG2RAD(a) (a * M_PI/180)

typedef struct {
    int x;
    int y;
} int2;

typedef struct {
    float x;
    float y;
} float2;

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
} Camera;

typedef struct {
    Object obj;
    Camera cam;
} Entity;

typedef struct {
    char r;
    char g;
    char b;
} Color;

typedef struct {
    float2 pos;
    float2 size;
    Color color;
} Wall;

typedef struct {
    int2 size;
    Color *pixels;
} Frame;


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

float2 sum_f2(float2 v1, float2 v2) {
    return (float2) {v1.x + v2.x, v1.y + v2.y};
}

float2 scale_f2(float2 v, float c) {
    return (float2) {v.x/c, v.y/c};
}

float2 normalize_f2(float2 v) {
    return scale_f2(v, Q_rsqrt(v.x*v.x + v.y*v.y));
}

float check_collision(float2 origin, float angle, float distance_resolution, float max_distance, Wall w) {
    float2 step = normalize_f2((float2){cosf(DEG2RAD(angle)), sinf(DEG2RAD(angle))});
    step = scale_f2(step, 1/distance_resolution);

    float2 point = origin;
    for (int i = 0; i < max_distance*distance_resolution; i++, point = sum_f2(point, step)) {
        if (point.x > w.pos.x || point.x < w.pos.x+w.size.x || point.y > w.pos.y || point.y < w.pos.y+w.size.y) {
            return sqrt(point.x*point.x + point.y*point.y);
        }
    }

    return -1;
}

int cast(Frame f, Camera c, Wall w) {
    float start_angle = c.dir_angle + (float)c.fov/2;
    float step_angle = (float) c.fov / c.n_rays;

    float angle = start_angle;
    int width = f.size.x / c.n_rays;

    Color to_fill = {0};
    for (int r = 0; r < c.n_rays; r++, angle -= step_angle) {
        float d = check_collision(c.pos, angle, c.distance_resolution, c.max_distance, w);
        to_fill = (Color){0, 0, 0};

        if (d >= 0) {
            to_fill = w.color;
        }

        for (int i = 0; i < f.size.y; i++) {
            for (int j = 0; j < width; j++) {
                f.pixels[C(r+j, i, f.size.x)] = to_fill;
            }
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

    for (int x = 0; x < f.size.x; x++){
        for (int y = 0; y < f.size.y; y++){
            fprintf(rendered_image, "%i %i %i ", f.pixels[C(x, y, f.size.x)].r,
                                                 f.pixels[C(x, y, f.size.x)].g,
                                                 f.pixels[C(x, y, f.size.x)].b);
        } 
        fprintf(rendered_image, "\n");
    }
    return 0;
}

void print_frame(Frame f) {
    for (int i = 0; i < f.size.x; i++) {
        for (int j = 0; j < f.size.y; j++) {
            Color cur = f.pixels[C(i, j, f.size.x)];
            printf("%d %d %d\t", cur.r, cur.g, cur.b);
        }
        printf("\n");
    }
}


int main(void) {
    float2 world_dim = {100, 100};
    int2 screen_dim = {1000, 1000};

    Frame test;
    test.size = screen_dim;

    test.pixels = calloc(screen_dim.x*screen_dim.y, sizeof(Color));
    //print_frame(test);

    Camera c;
    c.pos = (float2) {world_dim.x/2, world_dim.y/2};
    c.dir_angle = 0;
    c.fov = 90;
    c.max_distance = 30;
    c.distance_resolution = 100;
    c.n_rays = screen_dim.x/4;

    Wall w; 
    w.pos = (float2) {world_dim.x*7/8, world_dim.y*7/8};
    w.size = (float2) {5, 1};
    w.color = (Color) {255, 255, 255};
    
    cast(test, c, w);

    //print_frame(test);
    write_image(test, "out.ppm");

    return 0;
}
