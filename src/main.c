#include "image.h"
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

    f.pixels = calloc(f.size.x*f.size.y, sizeof(Color3));
    for (int i = 0; i < f.size.x*f.size.y; i++) {
        f.pixels[i].a = 255;
    }

    Frame downscaled;
    downscaled.size = (int2) {100, 100};

    downscaled.pixels = calloc(downscaled.size.x*downscaled.size.y, sizeof(Color3));
    for (int i = 0; i < downscaled.size.x*downscaled.size.y; i++) {
        downscaled.pixels[i].a = 255;
    }

    Camera_ c;
    c.pos = (float2) {world_dim.x/3 , world_dim.y/1.4};
    c.dir_angle = -15;
    c.fov = 70;
    c.max_distance = world_dim.x;
    c.distance_resolution = 10;
    c.n_rays = screen_dim.x;

    // load test scene
    WallNode *head = test_scene(world_dim);

    char frame_ascii[downscaled.size.x*downscaled.size.y + downscaled.size.y + 1];
    frame_ascii[sizeof(frame_ascii)] = 0; // set null char
    
    cast(f, c, head, (float2) {screen_dim.x / world_dim.x, screen_dim.y / world_dim.y } );
    // downscale(downscaled.pixels, f, downscaled.size.x, downscaled.size.y);
    // printf("%s", ascii(frame_ascii, downscaled));

#if GAMING_MODE
    InitWindow(screen_dim.x, screen_dim.y, "raycast visualization");
    SetTargetFPS(240);
    SetWindowMonitor(2);

    float movespeed = 20;
    float lookspeed = 100;
    Font font = LoadFont("/usr/share/fonts/TTF/LiterationMonoNerdFont-Regular.ttf");
    // Font font = GetFontDefault();

    int ENABLED_ASCII_PREVIEW = 0;
    int ENABLED_PIXEL_PREVIEW = 1;
    int ENABLED_DOWNSCALED_PREVIEW = 0;

    while (!WindowShouldClose()){
        float d = GetFrameTime();
        float2 dir = (float2){cosf(DEG2RAD(c.dir_angle)), sinf(DEG2RAD(c.dir_angle))};

        Vector2 pointer_r = GetMousePosition();
        int2 pointer;
        pointer.x = (int) pointer_r.x;
        pointer.y = (int) pointer_r.y;

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

        if (IsKeyPressed(KEY_F1))
            ENABLED_ASCII_PREVIEW = !ENABLED_ASCII_PREVIEW;
        if (IsKeyPressed(KEY_F2))
            ENABLED_PIXEL_PREVIEW = !ENABLED_PIXEL_PREVIEW;
        if (IsKeyPressed(KEY_F3))
            ENABLED_DOWNSCALED_PREVIEW = !ENABLED_DOWNSCALED_PREVIEW;


        cast(f, c, head, (float2) {screen_dim.x / world_dim.x, screen_dim.y / world_dim.y } );
        Image screenImage = { .data = f.pixels, .width = f.size.x, .height = f.size.y, .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, .mipmaps = 1 };
        Texture2D screenTexture = LoadTextureFromImage(screenImage);

        downscale(downscaled.pixels, f, downscaled.size.x, downscaled.size.y);
        Image downscaled_screenImage = { .data = downscaled.pixels, .width = downscaled.size.x, .height = downscaled.size.y, .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, .mipmaps = 1 };
        Texture2D downscaled_screenTexture = LoadTextureFromImage(downscaled_screenImage);

        ascii(frame_ascii, downscaled);
        BeginDrawing();
        ClearBackground(BLACK);
        // DrawTexture(screenTexture, 0, 0, WHITE);
        // DrawTexturePro(screenTexture, 
        //         (Rectangle) {.x = 0, .y = 0, .width = f.size.x, .height = f.size.y}, 
        //         (Rectangle) {.x = 0, .y = f.size.y - (float)f.size.y/3, .width = (float)f.size.x/3, .height = (float)f.size.y/3}, (Vector2){0, 0}, 0, RAYWHITE);

        if (ENABLED_DOWNSCALED_PREVIEW)
            DrawTexturePro(downscaled_screenTexture, 
                (Rectangle) {.x = 0, .y = 0, .width = downscaled.size.x, .height = downscaled.size.y}, 
                (Rectangle) {.x = 0, .y = 0, .width = f.size.x, .height = f.size.y}, (Vector2){0, 0}, 0, RAYWHITE);

        if (ENABLED_PIXEL_PREVIEW)
            DrawTexturePro(screenTexture, 
                (Rectangle) {.x = 0, .y = 0, .width = f.size.x, .height = f.size.y}, 
                (Rectangle) {.x = 0, .y = 0, .width = 100, .height = 100}, (Vector2){0, 0}, 0, RAYWHITE);

        if (ENABLED_ASCII_PREVIEW)
            DrawTextEx(font, frame_ascii, (Vector2) {0}, 8, 6, GRAY);
        // DrawText(frame_ascii, 0, 0, 24, BLACK);

        EndDrawing();
        UnloadTexture(screenTexture);
        UnloadTexture(downscaled_screenTexture);

        print_to_term(frame_ascii);
    }
#endif

    //write_image(f, "out.ppm");

    free(f.pixels);
    return 0;
}
