#include "dungeon.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "XoshiroCpp.hpp"
#include <deque>
#include <iostream>
#include <math.h>

namespace xrand = XoshiroCpp;

dungeon_level::dungeon_level(uint64_t level) : levelId(level) {
    rng = xrand::Xoshiro256PlusPlus(level);
    generate();
}

dungeon_level::dungeon_level(dungeon& d, uint64_t level) : levelId(level) {
    rng = xrand::Xoshiro256PlusPlus(level);
    generate();
}

dungeon_level::~dungeon_level() {}

void dungeon_level::generate() {
    // https://www.gamedeveloper.com/programming/procedural-dungeon-generation-algorithm

    std::deque<rect> rooms;

    std::cout << "[";
    for (int i=0; i<50; i++) {
        int w = rng() % 25 + 3;
        int h = rng() % 25 + 3;

        rect tmpRect = rect{-(w/2), -(h/2), w, h};

        std::deque<rect> overlaps = utils::overlappingRects(tmpRect, rooms);
        while (overlaps.size() > 0) {
            int pushX, pushY;

            float tl_distance = utils::distance({tmpRect.x, tmpRect.y}, {overlaps[0].x, overlaps[0].y});
            float tr_distance = utils::distance({tmpRect.x+tmpRect.w, tmpRect.y}, {overlaps[0].x+overlaps[0].w, overlaps[0].y});
            float br_distance = utils::distance({tmpRect.x+tmpRect.w, tmpRect.y+tmpRect.h}, {overlaps[0].x+overlaps[0].w, overlaps[0].y+overlaps[0].h});
            float bl_distance = utils::distance({tmpRect.x, tmpRect.y+tmpRect.h}, {overlaps[0].x, overlaps[0].y+overlaps[0].h});

            if (tl_distance > tr_distance && tl_distance > br_distance && tl_distance > bl_distance) {
                point x = {overlaps[0].x-tmpRect.x, overlaps[0].y-tmpRect.y};

            } else if (tr_distance > tl_distance && tr_distance > br_distance && tr_distance > bl_distance) {
                point x = {overlaps[0].x+overlaps[0].w-(tmpRect.x+tmpRect.w), overlaps[0].y-tmpRect.y};

            } else if (br_distance > tl_distance && br_distance > tr_distance && br_distance > bl_distance) {
                point x = {overlaps[0].x+overlaps[0].w-(tmpRect.x+tmpRect.w), overlaps[0].y+overlaps[0].h-(tmpRect.y+tmpRect.h)};

            } else if (bl_distance > tl_distance && bl_distance > tr_distance && bl_distance > br_distance) {
                point x = {overlaps[0].x-tmpRect.x, overlaps[0].y+overlaps[0].h-(tmpRect.y+tmpRect.h)};

            } else {
                pushX = 1;
                pushY = 1;
            }

            tmpRect.x += pushX;
            tmpRect.y += pushY;

            std::cout << "(" << pushX << ", " << pushY << ") (" << tmpRect.x << ", " << tmpRect.y << ", " << tmpRect.w << ", " << tmpRect.h << ")\n";

            std::deque<rect> overlaps = utils::overlappingRects(tmpRect, rooms);
        }

        rooms.push_back(tmpRect);

        std::cout << tmpRect.tostring() << "," << std::endl;
    }
    std::cout << "]";

    //std::cout << "[";
    //std::cout << rooms[0].tostring();
    //rooms.pop_front();
    //for (rect room : rooms) {
    //    std::cout << "," << room.tostring();
    //}
    //std::cout << "]";
}