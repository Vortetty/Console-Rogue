# Get a list from sys.argv and parse as json
import sys
import json
from PIL import Image, ImageDraw
import colorsys
import random

stdin = sys.stdin.read().replace("\n","").replace(",]","]")
print(stdin)
data  = json.loads(stdin)
cells = data["cells"]
rooms = data["rooms"]
tris  = data["triangles"]

# Given a list of rooms in the format [x, y, w, h], get the minimum x and y, and the maximum x and y
maxBounds = [
    min(cells, key=lambda x: x[0]),
    min(cells, key=lambda x: x[1]),
    max(cells, key=lambda x: x[0]+x[2]),
    max(cells, key=lambda x: x[1]+x[3])    
]
print(maxBounds)

adjustX =  maxBounds[0][0]
adjustY =  maxBounds[1][1]
imWidth =  maxBounds[2][0] + maxBounds[2][2]
imHeight = maxBounds[3][1] + maxBounds[3][3]

# Given a list of rooms in the format [x, y, w, h], and what to adjust the x and y by to fit on an image, create a PIL image and draw the rooms on it
roomlen = len(rooms)
print(imWidth, imHeight)
im = Image.new('RGBA', (imWidth + 2, imHeight + 2))
draw = ImageDraw.Draw(im)
for n,cell in enumerate(cells):
    rect = [
        cell[0] - adjustX + 1,
        cell[1] - adjustY + 1,
        cell[0] - adjustX + 1 + cell[2],
        cell[1] - adjustY + 1 + cell[3]
    ]
    draw.rectangle(rect, fill=(50,50,50,255))
    
for n,room in enumerate(rooms):
    rect = [
        room[0] - adjustX + 1,
        room[1] - adjustY + 1,
        room[0] - adjustX + 1 + room[2],
        room[1] - adjustY + 1 + room[3]
    ]
    draw.rectangle(rect, fill=(100,100,100,255))
    
for n,tri in enumerate(tris):
    draw.line(( tri[0][0]-adjustX, tri[0][1]-adjustY, tri[1][0]-adjustX, tri[1][1]-adjustY ), fill=(100,150,255,255))
    draw.line(( tri[1][0]-adjustX, tri[1][1]-adjustY, tri[2][0]-adjustX, tri[2][1]-adjustY ), fill=(100,150,255,255))
    draw.line(( tri[2][0]-adjustX, tri[2][1]-adjustY, tri[0][0]-adjustX, tri[0][1]-adjustY ), fill=(100,150,255,255))
#im.resize((imWidth*10, imHeight*10), Image.NEAREST)
im.save('rooms.png')
