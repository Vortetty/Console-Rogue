# Get a list from sys.argv and parse as json
import sys
import json
from PIL import Image, ImageDraw
import colorsys

stdin = sys.stdin.read().replace("\n","").replace(",]","]")
print(stdin)
rooms = json.loads(stdin)

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
im = Image.new('RGBA', (abs(imWidth)+1000, abs(imHeight)+1000))
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
