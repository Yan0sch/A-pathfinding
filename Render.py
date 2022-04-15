from ctypes import cdll
import pygame
from math import sqrt


class Node:
    normal = 0
    start = 1
    dest = 2
    path = 3
    wall = 4


def get_color(val: int, max):
    val = int(val / max * 0xff)
    r = val
    g = 0xff - val

    t = (0xff * 2 - (r + g)) // 2
    r += t
    g += t

    if r > 0xff:
        r = 0xff
    if g > 0xff:
        g = 0xff

    if r < 0: r = 0
    if g < 0: g = 0
    return r, g, 0


class Render:

    def __init__(self, dll: cdll, root: pygame.Surface):
        self.dll = dll
        self.__root = root
        self.size = dll.get_size()
        self.init()
        self.max_dist = self.dll.get_shortest_distance()

    def init(self):
        self.res = pygame.Surface((self.size * 10, self.size * 10))
        self.wall = pygame.Surface((10, 10))
        self.wall.fill(pygame.Color(0x000000ff))

        self.start = pygame.Surface((10, 10))
        self.start.fill(pygame.Color(0x00ffffff))
        #pygame.draw.circle(self.start, pygame.Color(0x0000ffff), (5, 5), 4)

        self.path = pygame.Surface((10, 10))
        self.path.fill(pygame.Color(0x0000ffff))

        self.dest = pygame.Surface((10, 10))
        self.dest.fill(pygame.Color(0xff00ffff))
        #pygame.draw.circle(self.dest, pygame.Color(0x0000ffff), (5, 5), 4)

        self.normal = pygame.Surface((10, 10))
        self.normal.fill(pygame.Color(0x00ffffff))

        self.res = pygame.Surface((self.size * 10, self.size * 10))

    def draw(self):
        self.res.fill(pygame.Color(0xffffffff))
        for y in range(self.size):
            for x in range(self.size):
                w = self.dll.get_weight(x, y)
                t = self.dll.get_type(x, y)
                s = self.dll.get_status(x, y)
                if t == Node.start:
                    self.res.blit(self.start, (x * 10, y * 10))
                elif t == Node.dest:
                    self.res.blit(self.dest, (x * 10, y * 10))
                elif t == Node.normal and w < 0xffff:
                    if s == 1:
                        self.normal.fill(
                            get_color(w, self.max_dist))
                    else:
                        self.normal.fill(pygame.Color(0x00ffffff))
                    self.res.blit(self.normal, (x * 10, y * 10))
                elif t == Node.path:
                    self.res.blit(self.path, (x * 10, y * 10))
                elif t == Node.wall:
                    self.res.blit(self.wall, (x * 10, y * 10))

        self.__root.blit(self.res, (0, 0))

    def render(self):
        self.draw()
