from threading import Thread
from time import time

import pygame

from Render import Render

from ctypes import *


lib = "lib/pathFinder.so"
dll = cdll.LoadLibrary(lib)
dll.init()
size = dll.get_size()

dll.set_start(40, 40)
dll.set_dest(60, 60)

for x in range(30, 50+1):
	dll.set_wall(x, 50)

for x in range(30, 50+1):
	dll.set_wall(50, x)

for x in range(32, 50+1):
	dll.set_wall(30, x)

for x in range(30, 50+1):
	dll.set_wall(x, 30)


pygame.init()
screen = pygame.display.set_mode((size * 10, size * 10))

renderer = Render(dll, screen)


# path_finder = Dijkstra(grid)


def render():
	done = False
	fps_font = pygame.font.SysFont("Droid Sans Mono", 20)
	fps = 0
	clock = pygame.time.Clock()
	MAX_FPS = 60

	while not done:
		t0 = time()
		clock.tick(MAX_FPS)

		renderer.render()

		screen.blit(fps_font.render("{:.1f} fps".format(fps), True, 0xff0000ff), (0, 0))

		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				done = True
				dll.stop()

		pygame.display.flip()
		fps = 1 / (time() - t0)
	
	
def main():
	t0 = time()
	dll.main()
	print("time: %f s"%(time() - t0))

if __name__ == "__main__":
	Thread(target=render).start()
	Thread(target=main).start()
