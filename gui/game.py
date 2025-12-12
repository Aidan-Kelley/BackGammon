from typing import List
import pygame

SCREEN_WIDTH = 640
SCREEN_HEIGHT = 640
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
stones : List['Stone'] = []

def gameLoop() -> bool:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            return True
    
    for stone in stones:
        stone.draw()

    screen.fill((0,0,0))
    if pygame.mouse.get_pressed()[0]:
        


    return True

class Stone:
    def __init__(self, x : int, y : int):

    def draw():
        pygame.draw.circle(screen, (255,255,255), pygame.mouse.get_pos(), 30)