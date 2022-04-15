#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grid.c"
#include <unistd.h>

char is_wall_nearby(Grid *grid, pos p)
{
    pos n = p;
    n.x += 1;
    if (check_pos(grid, n) && (grid->grid + pos2idx(grid, n))->t == wall)
        return 1;
    n = p;
    n.x -= 1;
    if (check_pos(grid, n) && (grid->grid + pos2idx(grid, n))->t == wall)
        return 1;
    n = p;
    n.y += 1;
    if (check_pos(grid, n) && (grid->grid + pos2idx(grid, n))->t == wall)
        return 1;
    n = p;
    n.y -= 1;
    if (check_pos(grid, n) && (grid->grid + pos2idx(grid, n))->t == wall)
        return 1;

    n = p;
    n.x += 1;
    n.y += 1;
    if (check_pos(grid, n) && (grid->grid + pos2idx(grid, n))->t == wall)
        return 1;

    n = p;
    n.x -= 1;
    n.y += 1;
    if (check_pos(grid, n) && (grid->grid + pos2idx(grid, n))->t == wall)
        return 1;

    n = p;
    n.x -= 1;
    n.y -= 1;
    if (check_pos(grid, n) && (grid->grid + pos2idx(grid, n))->t == wall)
        return 1;

    n = p;
    n.x += 1;
    n.y -= 1;
    if (check_pos(grid, n) && (grid->grid + pos2idx(grid, n))->t == wall)
        return 1;

    return 0;
}

char add_node(Grid *grid, pos *queue, int *idx, node *current, pos next_pos)
{
    if (pos2idx(grid, next_pos) == pos2idx(grid, grid->dest))
    {
        (grid->grid + pos2idx(grid, grid->dest))->prev = current;
        return 1;
    }

    static __uint32_t new_weight;
    static node *next;
    next = grid->grid + pos2idx(grid, next_pos);
    if (check_pos(grid, next_pos) && next->t != wall && !next->done)
    {
        if (next->weight == 0xffff)
        {
            queue[(*idx)++] = next_pos;
            next->distance = round(sqrt(pow(grid->dest.x - next_pos.x, 2) + pow(grid->dest.y - next_pos.y, 2)) * 2);
        }

        if (is_wall_nearby(grid, next_pos) == 1)
            new_weight = current->weight + 10;
        else
            new_weight = current->weight + 1;

        if (next->weight >= new_weight)
        {
            next->weight = new_weight;
            next->prev = current;
        }
    }
    return 0;
}

char is_pos_equal(pos a, pos b)
{
    return a.x == b.x && a.y == b.y;
}

char get_neightbours(Grid *grid, pos *queue, int *idx, int curr_idx)
{
    pos *current_pos = queue + curr_idx;
    node *current = grid->grid + pos2idx(grid, *current_pos);
    current->done = 1;
    //printf("done: %i\n", current->done);

    pos next = *current_pos;
    next.x += 1;
    if (add_node(grid, queue, idx, current, next))
        return 1;

    next = *current_pos;
    next.x -= 1;
    if (add_node(grid, queue, idx, current, next))
        return 1;

    next = *current_pos;
    next.y += 1;
    if (add_node(grid, queue, idx, current, next))
        return 1;

    next = *current_pos;
    next.y -= 1;
    if (add_node(grid, queue, idx, current, next))
        return 1;

    if (add_node(grid, queue, idx, current, next))
        return 1;

    return 0;
}

char compare_weight(Grid *grid, pos *queue, int idx1, int idx2, float dist_weight)
{
    static float value_1, value_2;
    static node *n1, *n2;
    n1 = grid->grid + pos2idx(grid, queue[idx1]);
    n2 = grid->grid + pos2idx(grid, queue[idx2]);

    //pos p1 = queue[idx1];
    //pos p2 = queue[idx2];
    /*float d1 = sqrt(pow(grid->dest.x - p1.x, 2) + pow(grid->dest.y - p1.y, 2)) * 2;
    float d2 = sqrt(pow(grid->dest.x - p2.x, 2) + pow(grid->dest.y - p2.y, 2)) * 2;*/

    value_1 = n1->weight + n1->distance;
    value_2 = n2->weight + n2->distance;

    //printf("x: %i, y: %i, done: %i, ", queue[idx2].x, queue[idx2].y, n2->done);
    //printf("x: %i, y: %i, done: %i\n", queue[idx1].x, queue[idx1].y, n1->done);
    return (value_1 > value_2 && !n2->done) || n1->done;
}

int get_next(Grid *grid, pos *queue, int idx, float dist_weight)
{
    int res = 0;
    for (int i = 0; i < idx; i++)
    {
        if (compare_weight(grid, queue, res, i, dist_weight))
            res = i;
    }

    if (grid->grid[pos2idx(grid, *(queue + res))].done)
        return -1;
    return res;
}

void mark_path(Grid *grid)
{
    node *current = (grid->grid + (grid->dest.x * grid->size + grid->dest.y))->prev;

    while (current->prev != NULL)
    {
        current->t = path;
        current = current->prev;
    }
}

char run = 1;

void stop()
{
    run = 0;
}

void PathFinder(Grid *grid)
{
    pos *queue = (pos *)malloc(sizeof(pos) * grid->size * grid->size);
    int idx = 0;
    int curr_idx = 0;
    queue[idx++] = grid->start;

    while (!get_neightbours(grid, queue, &idx, curr_idx) && run && curr_idx >= 0)
    {
        curr_idx = get_next(grid, queue, idx, 1);
        usleep(10000);
    }

    printf("num of nodes: %i\n", idx);

    if (curr_idx < 0)
    {
        printf("No path could be found!\n");
        return;
    }

    mark_path(grid);
}