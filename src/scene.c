#include <stdlib.h>
#include "types.h"
#include "scene.h"

WallNode *add_wall(WallNode *tail, Wall w) {
    if (!tail) return NULL;

    WallNode *wn = malloc(sizeof(*wn));
    if (!wn) return NULL;

    wn->w = w;
    wn->next = NULL;
    tail->next = wn;

    return wn;
}

WallNode *test_scene(float2 world_dim) {
    Wall w; 
    w.size = (float2) {10, 20};
    w.pos = (float2) {world_dim.x*5.2/10, world_dim.y/1.2 - w.size.y/2};
    w.color = (Color3) {255, 255, 255};

    WallNode *head = malloc(sizeof(*head));
    if (!head) return NULL;
    head->w = w;

    WallNode *tail = head;

    w.size = (float2) {5, 30};
    w.pos = (float2) {world_dim.x*8.5/10, world_dim.y/1.6 - w.size.y/2};
    w.color = (Color3) {255, 0, 255};

    tail = add_wall(tail, w);

    w.size = (float2) {10, 30};
    w.pos = (float2) {world_dim.x*9/10, world_dim.y/3 - w.size.y/1.5};
    w.color = (Color3) {255, 0, 0};

    tail = add_wall(tail, w);

    return head;
}
