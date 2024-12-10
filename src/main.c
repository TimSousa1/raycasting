#include "image.h"
#include "ray.h"
#include "scene.h"
#include "ascii.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef GAMING_MODE
#include <raylib.h>
#endif

#ifdef ASCII
struct timespec time_diff(struct timespec *a, struct timespec *b){
    struct timespec diff;

    diff.tv_sec = a->tv_sec - b->tv_sec;
    diff.tv_nsec = a->tv_nsec - b->tv_nsec;

    return diff;
}

double read_time(struct timespec *a){
    double s;
    s = a->tv_sec;
    s += a->tv_nsec / 1e9;

    return s;
}
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

#ifdef ASCII
#include "term.h"
#include <time.h>
#include <termios.h>
#include <unistd.h>
#define FRAMETIME (1.0/120)      // time per frame in seconds (must be <1)

    // player attributes
    float movespeed = 20*10;
    float lookspeed = 100*10;

    // TAKEN FROM main.c AT https://github.com/timsousa1/snake
    // getting terminal ready
    struct termios original;
    enableRAW(&original, FRAMETIME*10); // (takes frametime in tenths of a sec)

    // frametime info
    struct timespec initFRAME;
    struct timespec midFRAME;
    struct timespec endFRAME;

    struct timespec ft;         // time that should be spent
    struct timespec pt;         // time spent processing
    struct timespec dt = {0};   // total time spent on frame
    struct timespec remaining;  // time to wait for next frame

    ft.tv_sec = 0;
    ft.tv_nsec = FRAMETIME * 1e9;

    char key = 0;
    while (1){
        // resetting player input
        key = 0;

        float d = read_time(&dt);
        clock_gettime(CLOCK_MONOTONIC, &initFRAME);
        clear_screen();

        // getting player input
        read(STDIN_FILENO, &key, sizeof(key));
        tcflush(STDIN_FILENO, TCIFLUSH);              // flushing what wasn't read
        // printf("got %c from stdin in RAW mode\n", key);

        if (key == 'q') break;
        
        ////////////// ACTUAL GAME LOOP ////////////////
        
        float2 dir = (float2){cosf(DEG2RAD(c.dir_angle)), sinf(DEG2RAD(c.dir_angle))};

        if (key == 'w') {
            c.pos = sum_f2(scale_f2(dir, d*movespeed), c.pos);
        }
        if (key == 's') {
            c.pos = sum_f2(scale_f2(dir, -d*movespeed), c.pos);
        }
        if (key == 'a') {
            c.dir_angle += lookspeed*d;
        }
        if (key == 'd') {
            c.dir_angle -= lookspeed*d;
        }

        cast(f, c, head, (float2) {screen_dim.x / world_dim.x, screen_dim.y / world_dim.y } );
        downscale(downscaled.pixels, f, downscaled.size.x, downscaled.size.y);
        ascii(frame_ascii, downscaled);
        print_to_term(frame_ascii);

        ///////////// CLOCK MUMBO JUMBO ///////////////////
        clock_gettime(CLOCK_MONOTONIC, &midFRAME);
        pt = time_diff(&midFRAME, &initFRAME);
        remaining = time_diff(&ft, &pt);

        nanosleep(&remaining, NULL);
        clock_gettime(CLOCK_MONOTONIC,&endFRAME);

        dt = time_diff(&endFRAME, &initFRAME);
        // printf("time taken on frame: %lf\n", read_time(&dt));
    }

    disableRAW(&original);

#endif

    //write_image(f, "out.ppm");

    free(f.pixels);
    return 0;
}
