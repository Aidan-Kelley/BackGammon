from enum import Enum
import struct
from typing import List
import pygame
from util import *

SCREEN_WIDTH = 640 * 2 + 100
SCREEN_HEIGHT = 640
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
stones : List['Stone'] = []
white_board : List[int] = [0,0,0,0,0,0,0]
brown_board : List[int] = [0,0,0,0,0,0,0]
updateText : bool = False

class Color(Enum):
    BROWN = (0x80,0x00,0x20)
    WHITE = (0xFF,0xFF,0xFF)

def init():
    global font
    global text_surface
    pygame.init()
    pygame.font.init()
    font = pygame.font.SysFont(None, 48)
    text_surface = font.render("%.5f" % (lookup(white_board)), True, Color.WHITE.value)
    for i in range(15):
        stones.append(Stone(50,600 - i*60,0, Color.WHITE))
        stones.append(Stone(50,600 - i*60,0, Color.BROWN))

def gameLoop() -> bool:
    global font, updateText, text_surface 
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            return False
    screen.fill((0,0,0))

    for stone in stones:
            stone.tick()
    if text_surface is not None:
        screen.blit(text_surface)
    if updateText:
        text_surface = font.render("Avg Moves: %.5f \n White Win Chance: %.2f%%" % (lookup(white_board),100 * chanceOfWinning(white_board,brown_board)), True, Color.WHITE
.value)
        updateText = False
    pygame.draw.line(screen, Color.WHITE.value, (640,640),(640,0), 3)
    return True

class Stone:
    x : int
    y : int
    yVel : int = 0
    floor : int = 600
    hitbox = pygame.Rect()
    dragging : bool = False
    color : Color
    colorOffset = 0

    def __init__(self, x : int, y : int, space : int, color : Color):
        self.color = color
        if color == Color.BROWN:
            self.colorOffset = 640
        self.space = space
        getBoard(self.color)[space] += 1
        self.floor = 660 - getBoard(self.color)[self.space] * STONE_RADIUS * 2
        self.x = x
        self.y = y

    def tick(self):
        global text_surface, updateText
        self.yVel += 1
        if pygame.mouse.get_pressed()[0] and (dist(pygame.mouse.get_pos(), (self.x + self.colorOffset,self.y)) < STONE_RADIUS or self.dragging):
            self.dragging = True
            if(not self.dragging):
                pass
        if pygame.mouse.get_just_released()[0] and self.dragging:
            self.dragging = False
            getBoard(self.color)[self.space] -= 1
            for stone in stones:
                if stone.color == self. color and stone.space == self.space and stone.floor < self.floor:
                    stone.floor += STONE_RADIUS * 2 
            self.space = self.x // 90
            getBoard(self.color)[self.space] += 1
            self.floor = 660 - getBoard(self.color)[self.space] * STONE_RADIUS * 2 
            updateText = True


        if self.dragging:
           self.x, self.y = pygame.mouse.get_pos()
           self.x -= self.colorOffset
           self.x = clamp(self.x,45,585)
           self.yVel = 0
        elif(self.y >= self.floor):
            self.y = self.floor
            self.yVel = 0

        self.y += self.yVel
        self.x = self.x - (self.x % 90) + 45
        pygame.draw.circle(screen, self.color
.value, (self.x + self.colorOffset,self.y), STONE_RADIUS)

        # 3. Blit the text surface to the screen
        
def getBoard(color : Color) -> List[int]:
    if color == Color.WHITE:
       return white_board
    return brown_board

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
    