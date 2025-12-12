import pygame
import game

running = True
clock = pygame.time.Clock() 
game.init()
while game.gameLoop():
    # Update the display to show the new drawing
    pygame.display.update()
    clock.tick(60)

pygame.quit()