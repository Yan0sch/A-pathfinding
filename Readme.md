# Pathfinding Simulation
This program uses a little c library to perform a A* pathfinding in a grid. This grid is displayed by a python (version 3.6.9) program. The heuristic is simply `weight + distance_to_target` and it also try to avoid contact with walls.

## Usage
`python main.py`

or

`python3 main.py`

If you change something in the c program part (files in the "lib" folder) you have to recompile this code:

`gcc -shared -fpic lib/main.c -o lib/pathFinder.so` (Note: this only works on Linux)