import pygame
import game

pygame.init()

running = True
clock = pygame.time.Clock() 

while game.gameLoop():
    # Update the display to show the new drawing
    pygame.display.update()
    clock.tick(60)

pygame.quit()