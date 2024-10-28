#include "ray.h"
#include "scene.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef VISUALIZE
#include <raylib.h>
#endif

int main(void) {
    float2 world_dim = {100, 100};
    int2 screen_dim = {1000, 1000};

#ifdef VISUALIZE
    InitWindow(screen_dim.x, screen_dim.y, "raycast visualization");
    SetTargetFPS(10);
    BeginDrawing();
    ClearBackground(GRAY);
    EndDrawing();
#endif

    Frame test;
    test.size = screen_dim;

    test.pixels = calloc(screen_dim.x*screen_dim.y, sizeof(Color3));
    for (int i = 0; i < screen_dim.x*screen_dim.y; i++) {
        test.pixels[i].a = 255;
    }

    Camera_ c;
    c.pos = (float2) {world_dim.x/3 , world_dim.y/1.4};
    c.dir_angle = -15;
    c.fov = 90;
    c.max_distance = screen_dim.x;
    c.distance_resolution = 20;
    c.n_rays = 250;

    WallNode *head = test_scene(world_dim);
    
#ifdef VISUALIZE
    cast(test, c, head, (float2) {screen_dim.x / world_dim.x, screen_dim.y / world_dim.y } );
#else
    cast(test, c, head);
#endif

    write_image(test, "out.ppm");

    free(test.pixels);
    return 0;
}
