import math

STONE_RADIUS = 20

def clamp(n, smallest, largest): return max(smallest, min(n, largest))

def dist(p1 : tuple[int, int], p2 : tuple[int, int]):
    return math.sqrt((p1[0]-p2[0])**2 + (p1[1]-p2[1])**2)