#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum
{
    normal = 0,
    start = 1,
    head = 1,
    dest = 2,
    food = 2,
    path = 3,
    wall = 4,
    snake = 4
} type;

typedef struct
{
    __uint8_t x, y;
} pos;

typedef struct _node
{
    __uint16_t weight;
    __uint32_t distance;
    type t;
    char done;
    struct _node *prev;
} node;

typedef struct
{
    size_t size;
    node *grid;
    pos start, dest;
} Grid;


Grid *get_grid(size_t size)
{
    static Grid result;
    result = (Grid){.size = size, .grid = (node *)malloc(sizeof(node) * size * size)};
    node init_node = (node){.weight = 0xffff, .done = 0, .t = normal, .prev = NULL, .distance=0xffff};

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            result.grid[x * size + y] = init_node;
        }
    }

    return &result;
}

int pos2idx(Grid * grid, pos p){
    return p.x * grid->size + p.y;
}

char check_pos(Grid * grid, pos p)
{
   return p.x >= 0 && p.x < grid->size && p.y >= 0 && p.y < grid->size;
}
