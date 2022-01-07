#include "utils.hpp"
#include <math.h>
#include <algorithm>

bool point::operator==(const point& other) const { return x == other.x && y == other.y; }
bool point::operator!=(const point& other) const { return x != other.x || y != other.y; }
bool point::operator<(const point& other) const { return x < other.x && y < other.y; }
bool point::operator>(const point& other) const { return x > other.x && y > other.y; }
bool point::operator<=(const point& other) const { return x <= other.x && y <= other.y; }
bool point::operator>=(const point& other) const { return x >= other.x && y >= other.y; }

bool rect::operator==(const rect& other) const { return x == other.x && y == other.y && w == other.w && h == other.h; }
bool rect::operator!=(const rect& other) const { return x != other.x || y != other.y || w != other.w || h != other.h; }
bool rect::operator<(const rect& other) const { return x < other.x && y < other.y && w < other.w && h < other.h; }
bool rect::operator>(const rect& other) const { return x > other.x && y > other.y && w > other.w && h > other.h; }
bool rect::operator<=(const rect& other) const { return x <= other.x && y <= other.y && w <= other.w && h <= other.h; }
bool rect::operator>=(const rect& other) const { return x >= other.x && y >= other.y && w >= other.w && h >= other.h; }

point rect::position() { return point{x, y}; }
point rect::center() { return point{x + (w / 2), y + (h / 2)}; }
point rect::size() { return point{w, h}; }


std::string rect::tostring() {
    return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + "]";
}

bool utils::rectsOverlap(rect a, rect b)  {
    // are the sides of one rectangle touching the other?

    if (a.x + a.w >= b.x &&    // r1 right edge past r2 left
        a.x <= b.x + b.w &&    // r1 left edge past r2 right
        a.y + a.h >= b.y &&    // r1 top edge past r2 bottom
        a.y <= b.y + b.h) {    // r1 bottom edge past r2 top
            return true;
    }
    return false;
}

bool utils::rectsOverlap(rect a, std::deque<rect> b) {
    for (auto& r : b) {
        if (utils::rectsOverlap(a, r)) {
            return true;
        }
    }
    return false;
}

std::deque<rect> utils::overlappingRects(rect a, std::deque<rect> b) {
    std::deque<rect> overlaps;
    for (auto& r : b) {
        if (utils::rectsOverlap(a, r)) {
            overlaps.push_back(r);
        }
    }
    return overlaps;
}

float utils::distance(point a, point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) * 1.0);
}



std::deque<rect> utils::binarySpacePartition(rect spaceToSplit, int minRoomSize) {
    std::deque<rect> rooms;
    std::deque<rect> outRooms;

    rooms.push_back(spaceToSplit);

    while (rooms.size() > 0) {
        rect currentRoom = rooms.front();
        rooms.pop_front();

        if (currentRoom.size() > point{minRoomSize, minRoomSize}) {
            if (rand() % 10 == 0 && currentRoom.size() <= point{minRoomSize * 4, minRoomSize * 4}) {
                outRooms.push_back(currentRoom);
                continue;
            }

            int splitDirection = rand() % 2;

            if (splitDirection == 0) {
                if (currentRoom.w >= minRoomSize*2) {
                    splitDirection = 0;
                } else if (currentRoom.h >= minRoomSize*2) {
                    splitDirection = 1;
                } else {
                    splitDirection = 2;
                }
            } else if (splitDirection == 1) {
                if (currentRoom.h >= minRoomSize*2) {
                    splitDirection = 1;
                } else if (currentRoom.w >= minRoomSize*2) {
                    splitDirection = 0;
                } else {
                    splitDirection = 2;
                }
            }

            if (splitDirection == 0) {
                int splitPoint = std::max(rand() % (currentRoom.w - 2) + 1, minRoomSize);

                rooms.push_back(rect{currentRoom.x, currentRoom.y, splitPoint, currentRoom.h});
                rooms.push_back(rect{currentRoom.x + splitPoint, currentRoom.y, currentRoom.w - splitPoint, currentRoom.h});
            } else if (splitDirection == 1) {
                int splitPoint = std::max(rand() % (currentRoom.h - 2) + 1, minRoomSize);

                rooms.push_back(rect{currentRoom.x, currentRoom.y, currentRoom.w, splitPoint});
                rooms.push_back(rect{currentRoom.x, currentRoom.y + splitPoint, currentRoom.w, currentRoom.h - splitPoint});
            } else {
                outRooms.push_back(currentRoom);
            }
        }
    }

    
    for (auto& r : outRooms) {
        r = rect{ r.x+1, r.y+1, r.w-2, r.h-2 };
    }

    return outRooms;
}
