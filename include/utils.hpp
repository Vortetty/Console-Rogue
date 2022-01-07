#pragma once
#include <deque>
#include <string>

struct point{
    int x, y;

    bool operator==(const point& other) const;
    bool operator!=(const point& other) const;
    bool operator<(const point& other) const;
    bool operator>(const point& other) const;
    bool operator<=(const point& other) const;
    bool operator>=(const point& other) const;
};

struct rect {
    int x, y, w, h;

    std::string tostring();
    point position();
    point center();
    point size();

    bool operator==(const rect& other) const;
    bool operator!=(const rect& other) const;
    bool operator<(const rect& other) const;
    bool operator>(const rect& other) const;
    bool operator<=(const rect& other) const;
    bool operator>=(const rect& other) const;
};

namespace utils {
    // Returns true if two rectangles (l1, r1) and (l2, r2)
    // overlap
    bool rectsOverlap(rect a, rect b);
    bool rectsOverlap(rect a, std::deque<rect> b);
    std::deque<rect> overlappingRects(rect a, std::deque<rect> b);
    float distance(point a, point b);

    std::deque<rect> binarySpacePartition(rect spaceToSplit, int minRoomSize);
}