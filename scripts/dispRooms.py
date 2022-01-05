# Get a list from sys.argv and parse as json
import sys
import json
from PIL import Image, ImageDraw
import colorsys

#stdin = sys.stdin.read().replace("\n","").replace(",]","]")
#print(stdin)
rooms = [[-3, -4, 6, 8],
[-19, -4, 8, 8],
[20, 1, 27, 11],
[10, -10, 9, 6],
[9, 15, 10, 13],
[24, -20, 23, 8],
[-9, 7, 12, 23],
[30, 26, 21, 26],
[-31, -20, 13, 15],
[56, 51, 24, 17],
[37, 77, 25, 20],
[7, 11, 4, 3],
[-3, -36, 6, 24],
[9, -24, 5, 11],
[-63, -52, 24, 20],
[5, -24, 3, 23],
[-43, 4, 20, 7],
[5, -46, 3, 17],
[-27, -62, 16, 26],
[-11, 52, 17, 22],
[9, -58, 10, 14],
[-43, 12, 20, 5],
[-101, -64, 22, 16],
[-1, -80, 18, 20],
[-57, -15, 12, 18]]  #json.loads(stdin)

# Given a list of rooms in the format [x, y, w, h], get the minimum x and y, and the maximum x and y
maxBounds = [
    min(rooms, key=lambda x: x[0]),
    min(rooms, key=lambda x: x[1]),
    max(rooms, key=lambda x: abs(x[0])+x[2]),
    max(rooms, key=lambda x: abs(x[1])+x[3])    
]
print(maxBounds)

adjustX =  maxBounds[0][0]
adjustY =  maxBounds[1][1]
imWidth =  maxBounds[2][0] + maxBounds[2][2]
imHeight = maxBounds[3][1] + maxBounds[3][3]

# Given a list of rooms in the format [x, y, w, h], and what to adjust the x and y by to fit on an image, create a PIL image and draw the rooms on it
roomlen = len(rooms)
print(imWidth, imHeight)
im = Image.new('RGBA', (abs(imWidth)+10, abs(imHeight)+10))
draw = ImageDraw.Draw(im)
for n,room in enumerate(rooms):
    rect = [
        room[0] - adjustX + 4,
        room[1] - adjustY + 4,
        room[0] - adjustX + room[2] + 4,
        room[1] - adjustY + room[3] + 4
    ]
    print(rect)
    draw.rectangle(rect, outline=tuple(int(i*255) for i in colorsys.hsv_to_rgb(n/roomlen, 1, 1)))
#im.resize((imWidth*10, imHeight*10), Image.NEAREST)
im.save('rooms.png')
