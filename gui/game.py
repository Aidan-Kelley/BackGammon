import struct
from typing import List
import pygame
from util import *

SCREEN_WIDTH = 640
SCREEN_HEIGHT = 640
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
stones : List['Stone'] = []
board : List[int] = [0,0,0,0,0,0,0]

def init():
    global font
    global text_surface
    pygame.init()
    pygame.font.init()
    font = pygame.font.SysFont(None, 48)
    text_surface = font.render("%.5f" % (lookup(board)), True, (255,255,255))
    for i in range(15):
        stones.append(Stone(50,600 - i*60,0))

def gameLoop() -> bool:
    global font
    global text_surface 
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            return False
    screen.fill((0,0,0))

    for stone in stones:
            stone.tick()
    if text_surface is not None:
        screen.blit(text_surface)



    return True

class Stone:
    x : int
    y : int
    yVel : int = 0
    floor : int = 600
    hitbox = pygame.Rect()
    dragging : bool = False

    def __init__(self, x : int, y : int, space : int):
        self.space = space
        board[space] += 1
        self.floor = 660 - board[self.space] * STONE_RADIUS * 2 
        self.x = x
        self.y = y

    def tick(self):
        global text_surface
        self.yVel += 1
        if pygame.mouse.get_just_pressed()[0] and (dist(pygame.mouse.get_pos(), (self.x,self.y)) < STONE_RADIUS or self.dragging):
            self.dragging = not self.dragging
            if(not self.dragging):
                board[self.space] -= 1
                for stone in stones:
                    if stone.space == self.space and stone.floor < self.floor:
                        stone.floor += STONE_RADIUS * 2 
                self.space = self.x // 90
                board[self.space] += 1
                text_surface = font.render("Avg Moves: %.5f \n Win%% against same board: %f" % (lookup(board),chanceOfWinning(board,board)), True, (255,255,255))
                self.floor = 660 - board[self.space] * STONE_RADIUS * 2 
              
                
        if self.dragging:
           self.x, self.y = pygame.mouse.get_pos()
           self.x = clamp(self.x,45,585)
           self.yVel = 0
        

        self.y += self.yVel
        if(self.y > self.floor):
            self.y = self.floor
            self.yVel = 0
        self.x = self.x - (self.x % 90) + 45
        pygame.draw.circle(screen, (255,255,255), (self.x ,self.y), STONE_RADIUS)

        # 3. Blit the text surface to the screen
        

def getBoardId(board : List[int]) -> int:
    result = 0
    for i in range(1,len(board)):
        result |= board[i] << ((i - 1) * 4)
    return result

def lookup(board : List[int]) -> float:
    with open("C:\\Users\\aidan\\Documents\\Small Code Projects\\BackGammon\\gui\\tyler.bin","rb") as f:
        f.seek(getBoardId(board) * 4)
        bytes = f.read(4)
        value = struct.unpack('<f', bytes)[0]
        return value

def chanceOfWinning(heroBoard, oppBoard):
    heroChances = getWinProbabilities(heroBoard)
    oppChances = getWinProbabilities(oppBoard)
    winChance = 0.0
    oppWinChance = 0.0
    for i in range(len(heroChances)):
        winChance += heroChances[i] * (1 - oppWinChance)
        if i < len(oppChances):
            oppWinChance += oppChances[i]
    return winChance

def getWinProbabilities(board : List[int]) -> List[float]:
    with open("C:\\Users\\aidan\\Documents\\Small Code Projects\\BackGammon\\gui\\percents.txt","r") as file:
        id = getBoardId(board)
        s = ""
        for line in file:
            if(int(line[:8]) == id):
                s = line
                break
        strfloats = s[8:-2].split(",")
        return list(map(float, strfloats))
    