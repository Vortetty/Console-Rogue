#include <math.h>
#include <algorithm>
#include "tile.hpp"
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"
#include "utils.hpp"

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

bool rect_set::operator==(const rect_set& other) const { return r1 == other.r1 && r2 == other.r2; }
bool rect_set::operator!=(const rect_set& other) const { return r1 != other.r1 || r2 != other.r2; }
bool rect_set::operator<(const rect_set& other) const { return r1 < other.r1 && r2 < other.r2; }
bool rect_set::operator>(const rect_set& other) const { return r1 > other.r1 && r2 > other.r2; }
bool rect_set::operator<=(const rect_set& other) const { return r1 <= other.r1 && r2 <= other.r2; }
bool rect_set::operator>=(const rect_set& other) const { return r1 >= other.r1 && r2 >= other.r2; }

bool line::operator==(const line& other) const { return a == other.a && b == other.b; }
bool line::operator!=(const line& other) const { return a != other.a || b != other.b; }
bool line::operator<(const line& other) const { return a < other.a && b < other.b; }
bool line::operator>(const line& other) const { return a > other.a && b > other.b; }
bool line::operator<=(const line& other) const { return a <= other.a && b <= other.b; }
bool line::operator>=(const line& other) const { return a >= other.a && b >= other.b; }
void line::drawOnGrid(tileset& tiles, tile_type type) { // Thicc bresenham, just doesn't cut off corners, complexity somewhere around O((x2-x1) + (y2-y1))
    int dx =  std::abs(b.x - a.x);
    int dy = -std::abs(b.y - a.y);
    int sx = a.x < b.x ? 1 : -1;
    int sy = a.y < b.y ? 1 : -1;
    int err = dx + dy, e2;

    for (;;) {
        tiles[a.x][a.y].type = type;

        if (a.x == b.x && a.y == b.y) break;

        e2 = 2 * err;

        // EITHER horizontal OR vertical step (but not both!)
        if (e2 > dy) { 
            err += dy;
            a.x += sx;
        } else if (e2 < dx) { // <--- this "else" makes the difference
            err += dx;
            a.y += sy;
        }
    }
}
void line::drawOnGridNoDiag(tileset& tiles, tile_type type, bool horizontalFirst) { //complexity somewhere around O((x2-x1) + (y2-y1))
    point curPos = point{a.x, a.y};

    tiles[a.x][a.y].type = tile_type::tile_stairs_up;
    tiles[b.x][b.y].type = tile_type::tile_stairs_down;


    if (horizontalFirst) {
        while (curPos.x != b.x) {
            tiles[curPos.x][curPos.y].type = type;
            curPos.x += a.x < b.x ? 1 : -1;
        }
        while (curPos.y != b.y) {
            tiles[curPos.x][curPos.y].type = type;
            curPos.y += a.y < b.y ? 1 : -1;
        }
    } else {
        while (curPos.y != b.y) {
            tiles[curPos.x][curPos.y].type = type;
            curPos.y += a.y < b.y ? 1 : -1;
        }
        while (curPos.x != b.x) {
            tiles[curPos.x][curPos.y].type = type;
            curPos.x += a.x < b.x ? 1 : -1;
        }
    }

    tiles[curPos.x][curPos.y].type = type;
}

point rect::position() { return point{x, y}; }
point rect::center() { return point{x + (w / 2), y + (h / 2)}; }
point rect::size() { return point{w, h}; }

std::string point::tostring() { 
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; 
}

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

// O(n)
bool utils::rectsOverlap(rect a, std::deque<rect> b) {
    for (auto& r : b) {
        if (utils::rectsOverlap(a, r)) {
            return true;
        }
    }
    return false;
}

// O(n)
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


// Time complexity is... i need to do testing
std::deque<rect> utils::binarySpacePartition(rect spaceToSplit, int minRoomSize, PractRand::RNGs::Polymorphic::sfc64 rng) {
    std::deque<rect> rooms;
    std::deque<rect> outRooms;

    rooms.push_back(spaceToSplit);

    while (rooms.size() > 0) {
        rect currentRoom = rooms.front();
        rooms.pop_front();

        if (currentRoom.size() > point{minRoomSize, minRoomSize}) {
            if (rng.raw64() % 8 == 0 && currentRoom.size() <= point{minRoomSize * 4, minRoomSize * 4}) {
                outRooms.push_back(currentRoom);
                continue;
            }

            int splitDirection = rng.raw64() % 2;

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
                int splitPoint = std::max((int)(rng.raw64() % (currentRoom.w - 2) + 1), minRoomSize);

                rooms.push_back(rect{currentRoom.x, currentRoom.y, splitPoint, currentRoom.h});
                rooms.push_back(rect{currentRoom.x + splitPoint, currentRoom.y, currentRoom.w - splitPoint, currentRoom.h});
            } else if (splitDirection == 1) {
                int splitPoint = std::max((int)(rng.raw64() % (currentRoom.h - 2) + 1), minRoomSize);

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
