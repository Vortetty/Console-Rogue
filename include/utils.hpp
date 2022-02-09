#pragma once
#include <deque>
#include <string>
#include "tile.hpp"
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"

struct point{
    int x, y;

    std::string tostring();

    bool operator==(const point& other) const;
    bool operator!=(const point& other) const;
    bool operator<(const point& other) const;
    bool operator>(const point& other) const;
    bool operator<=(const point& other) const;
    bool operator>=(const point& other) const;
};
typedef point vec2;

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

struct rect_set {
    rect r1, r2;

    bool operator==(const rect_set& other) const;
    bool operator!=(const rect_set& other) const;
    bool operator<(const rect_set& other) const;
    bool operator>(const rect_set& other) const;
    bool operator<=(const rect_set& other) const;
    bool operator>=(const rect_set& other) const;
};

struct line {
    point a, b;

    bool operator==(const line& other) const;
    bool operator!=(const line& other) const;
    bool operator<(const line& other) const;
    bool operator>(const line& other) const;
    bool operator<=(const line& other) const;
    bool operator>=(const line& other) const;

    void drawOnGrid(tileset& tiles, tile_type type);
    void drawOnGridNoDiag(tileset& tiles, tile_type type, bool horizontalFirst);
};

namespace utils {
    // Returns true if two rectangles (l1, r1) and (l2, r2)
    // overlap
    bool rectsOverlap(rect a, rect b);
    bool rectsOverlap(rect a, std::deque<rect> b);
    std::deque<rect> overlappingRects(rect a, std::deque<rect> b);
    float distance(point a, point b);

    template <typename T>
    bool inRange(T value, T min, T max) {
        return value >= min && value <= max;
    }

    template <typename T>
    T overlap(T min1, T max1, T min2, T max2) {
        return std::abs(std::max(0, std::min(max1, max2) - std::max(min1, min2)));
    }

    std::deque<rect> binarySpacePartition(rect spaceToSplit, int minRoomSize, PractRand::RNGs::Polymorphic::sfc64 rng);
}