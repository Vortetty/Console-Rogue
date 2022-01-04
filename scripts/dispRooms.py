# Get a list from sys.argv and parse as json
import sys
import json
from PIL import Image, ImageDraw
import colorsys

stdin = sys.stdin.read()
print(stdin)
rooms = json.loads(stdin)

# Given a list of rooms in the format [x, y, w, h], get the minimum x and y, and the maximum x and y
maxBounds = [
    min(rooms, key=lambda x: x[0])[0],
    min(rooms, key=lambda x: x[1])[1],
    max(rooms, key=lambda x: x[2])[2],
    max(rooms, key=lambda x: x[3])[3]    
]
print(maxBounds)

adjustX = maxBounds[0]
adjustY = maxBounds[1]
imWidth = maxBounds[2]
imHeight = maxBounds[3]

# Given a list of rooms in the format [x, y, w, h], and what to adjust the x and y by to fit on an image, create a PIL image and draw the rooms on it
roomlen = len(rooms)
im = Image.new('RGBA', (imWidth+10, imHeight+10))
draw = ImageDraw.Draw(im)
for n,room in enumerate(rooms):
    rect = [
        room[0] - adjustX + 4,
        room[1] - adjustY + 4,
        room[0] - adjustX + room[2] + 4,
        room[1] - adjustY + room[3] + 4
    ]
    draw.rectangle(rect, outline=tuple(int(i) for i in colorsys.hsv_to_rgb(n/roomlen, 255, 255)))
#im.resize((imWidth*10, imHeight*10), Image.NEAREST)
im.save('rooms.png')
