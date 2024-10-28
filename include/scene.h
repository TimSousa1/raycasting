#ifndef TIM_SCENE_H
#define TIM_SCENE_H
#include "types.h"

typedef struct {
    float2 pos;
    float2 size;
    Color3 color;
} Wall;

typedef struct _wall_node {
    Wall w;
    struct _wall_node *next;
} WallNode;

WallNode *add_wall(WallNode *tail, Wall w);
WallNode *test_scene(float2 world_dim);

#endif
