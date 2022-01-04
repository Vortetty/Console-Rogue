#pragma once
#include <deque>
#include <string>

struct point{
    int x, y;
};

struct rect {
    int x, y, w, h;

    std::string tostring();
};

namespace utils {
    // Returns true if two rectangles (l1, r1) and (l2, r2)
    // overlap
    bool rectsOverlap(rect a, rect b);
    bool rectsOverlap(rect a, std::deque<rect> b);
    std::deque<rect> overlappingRects(rect a, std::deque<rect> b);
    float distance(point a, point b);
}