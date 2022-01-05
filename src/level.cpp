#include "dungeon.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "XoshiroCpp.hpp"
#include <deque>
#include <iostream>
#include <math.h>
#include <algorithm>

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
            int pushX=1, pushY=1;

            float tl_distance = utils::distance({tmpRect.x, tmpRect.y}, {overlaps[0].x, overlaps[0].y});
            float tr_distance = utils::distance({tmpRect.x+tmpRect.w, tmpRect.y}, {overlaps[0].x+overlaps[0].w, overlaps[0].y});
            float br_distance = utils::distance({tmpRect.x+tmpRect.w, tmpRect.y+tmpRect.h}, {overlaps[0].x+overlaps[0].w, overlaps[0].y+overlaps[0].h});
            float bl_distance = utils::distance({tmpRect.x, tmpRect.y+tmpRect.h}, {overlaps[0].x, overlaps[0].y+overlaps[0].h});

            float maxDistance = std::max(tl_distance, std::max(tr_distance, std::max(br_distance, bl_distance)));

            if (maxDistance == tl_distance) {
                point tmp_tl = point{tmpRect.x+tmpRect.w, tmpRect.y+tmpRect.h};
                point overlap_br = point{overlaps[0].x, overlaps[0].y};

                pushX = tmp_tl.x - overlap_br.x;
                pushY = tmp_tl.y - overlap_br.y;
            } else if (maxDistance == br_distance) {
                point tmp_br = point{tmpRect.x, tmpRect.y};
                point overlap_tl = point{overlaps[0].x+overlaps[0].w, overlaps[0].y+overlaps[0].h};

                pushX = tmp_br.x - overlap_tl.x;
                pushY = tmp_br.y - overlap_tl.y;
            } else if (maxDistance == tr_distance) {
                point tmp_tr = point{tmpRect.x, tmpRect.y+tmpRect.h};
                point overlap_bl = point{overlaps[0].x+overlaps[0].w, overlaps[0].y};

                pushX = tmp_tr.x - overlap_bl.x;
                pushY = tmp_tr.y - overlap_bl.y;
            } else if (maxDistance == bl_distance) {
                point tmp_bl = point{tmpRect.x+tmpRect.w, tmpRect.y};
                point overlap_tr = point{overlaps[0].x, overlaps[0].y+overlaps[0].h};

                pushX = tmp_bl.x - overlap_tr.x;
                pushY = tmp_bl.y - overlap_tr.y;
            }

            tmpRect.x += pushX != 0 ? pushX : 1;
            tmpRect.y += pushY != 0 ? pushY : 1;
            overlaps = utils::overlappingRects(tmpRect, rooms);

            //std::cout << i << " (" << pushX << ", " << pushY << ") (" << tmpRect.x << ", " << tmpRect.y << ", " << tmpRect.w << ", " << tmpRect.h << ")\n";
            //for (rect overlap : overlaps) {
            //    std::cout << "    " << overlap.x << ", " << overlap.y << ", " << overlap.w << ", " << overlap.h << "\n";
            //}
        }
        std::cout << i << "\n";

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