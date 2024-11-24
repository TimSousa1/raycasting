#include "ray.h"
#include "scene.h"
#include "ascii.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef GAMING_MODE
#include <raylib.h>
#endif

int main(void) {
    float2 world_dim = {100, 100};
    int2 screen_dim = {1000, 1000};

    Frame f;
    f.size = screen_dim;

    f.pixels = calloc(screen_dim.x*screen_dim.y, sizeof(Color3));
    for (int i = 0; i < screen_dim.x*screen_dim.y; i++) {
        f.pixels[i].a = 255;
    }

    Camera_ c;
    c.pos = (float2) {world_dim.x/3 , world_dim.y/1.4};
    c.dir_angle = -15;
    c.fov = 70;
    c.max_distance = world_dim.x;
    c.distance_resolution = 10;
    c.n_rays = screen_dim.x;

    WallNode *head = test_scene(world_dim);
    
    cast(f, c, head, (float2) {screen_dim.x / world_dim.x, screen_dim.y / world_dim.y } );

#if GAMING_MODE
    InitWindow(screen_dim.x, screen_dim.y, "raycast visualization");
    SetTargetFPS(240);
    SetWindowMonitor(2);
    char frame_ascii[100*100 + 100];

    float movespeed = 20;
    float lookspeed = 100;
    Font font = LoadFont("/usr/share/fonts/TTF/LiterationMonoNerdFont-Regular.ttf");
    while (!WindowShouldClose()){
        float d = GetFrameTime();
        float2 dir = (float2){cosf(DEG2RAD(c.dir_angle)), sinf(DEG2RAD(c.dir_angle))};

        if (IsKeyDown(KEY_W)) {
            c.pos = sum_f2(scale_f2(dir, d*movespeed), c.pos);
        }
        if (IsKeyDown(KEY_S)) {
            c.pos = sum_f2(scale_f2(dir, -d*movespeed), c.pos);
        }
        if (IsKeyDown(KEY_A)) {
            c.dir_angle += lookspeed*d;
        }
        if (IsKeyDown(KEY_D)) {
            c.dir_angle -= lookspeed*d;
        }

        cast(f, c, head, (float2) {screen_dim.x / world_dim.x, screen_dim.y / world_dim.y } );
        Image screenImage = { .data = f.pixels, .width = f.size.x, .height = f.size.y, .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, .mipmaps = 1 };
        Texture2D screenTexture = LoadTextureFromImage(screenImage);

        ascii(frame_ascii, 50, 50, f);
        BeginDrawing();
        ClearBackground(BLACK);
        // DrawTexture(screenTexture, 0, 0, WHITE);
        // DrawTexturePro(screenTexture, 
        //         (Rectangle) {.x = 0, .y = 0, .width = f.size.x, .height = f.size.y}, 
        //         (Rectangle) {.x = 0, .y = f.size.y - (float)f.size.y/3, .width = (float)f.size.x/3, .height = (float)f.size.y/3}, (Vector2){0, 0}, 0, RAYWHITE);

        DrawTexturePro(screenTexture, 
                (Rectangle) {.x = 0, .y = 0, .width = f.size.x, .height = f.size.y}, 
                (Rectangle) {.x = 0, .y = 0, .width = 750, .height = 755}, (Vector2){0, 0}, 0, RAYWHITE);

        DrawTextEx(font, frame_ascii, (Vector2) {0}, 24, 3, GRAY);
        // DrawText(frame_ascii, 0, 0, 24, BLACK);

        EndDrawing();
        UnloadTexture(screenTexture);
    }
#endif

    //write_image(f, "out.ppm");

    free(f.pixels);
    return 0;
}
