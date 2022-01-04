#include "utils.hpp"
#include <math.h>

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
