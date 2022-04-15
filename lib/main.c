#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grid.c"
#include "path_finder.c"

const size_t size = 100;
Grid *grid = NULL;

int main()
{
    PathFinder(grid);
    return EXIT_SUCCESS;
}

/*
The following methods are to communicate with the python program (ctypes package).
They only contain 'standard' data types like int.
You also have to be careful with 'unsigned' types because they don't really exist in python
*/



void set_start(__uint16_t x, __uint16_t y)
{
    grid->start = (pos){.x = x, .y = y};
    grid->grid[x * grid->size + y].t = start;
    grid->grid[x * grid->size + y].weight = 0;
}

void set_dest(__uint16_t x, __uint16_t y)
{
    grid->dest = (pos){.x = x, .y = y};
    grid->grid[x * grid->size + y].t = dest;
}

void set_wall(__uint16_t x, __uint16_t y)
{
    grid->grid[x * grid->size + y].t = wall;
}

int get_shortest_distance()
{
   return round(sqrt(pow(grid->start.x - grid->dest.x, 2) + pow(grid->start.y - grid->dest.y, 2)));
}

void init()
{
    grid = get_grid(size);
}

int get_weight(__uint8_t x, __uint8_t y)
{
    if (x > size || x < 0)
        return 0xff;
    if (y > size || y < 0)
        return 0xff;
    return (int)grid->grid[x * size + y].distance - grid->grid[x * size + y].weight;
}

char get_status(__uint8_t x, __uint8_t y)
{
    if (x > size || x < 0)
        return 0xff;
    if (y > size || y < 0)
        return 0xff;
    return grid->grid[x * size + y].done;
}

__uint8_t get_type(__uint8_t x, __uint8_t y)
{
    if (x > size || x < 0)
        return 0xff;
    if (y > size || y < 0)
        return 0xff;
    return (__uint8_t)grid->grid[x * size + y].t;
}

size_t get_size()
{
    return size;
}
