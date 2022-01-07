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
    // Binary space partitioning
    // Minimum room size will be 7x7 
    // Maximum room size will be 27x27
    // Minimum number of rooms will be 10
    // Maximum number of rooms will be 50
    // Space to be partitioned will be 1000x1000
    const int min_room_size = 10;
    const int space_width = 200;
    const int space_height = 200;

    std::deque<rect> tmprooms = utils::binarySpacePartition(rect{0, 0, space_width, space_height}, min_room_size);
    std::deque<rect> rooms;

    for (auto& r : tmprooms) {
        rooms.push_back( rect{ r.x+1, r.y+1, r.w-2, r.h-2 } );
    }

    std::cout << "[";
    std::cout << rooms[0].tostring();
    rooms.pop_front();
    for (rect room : rooms) {
        std::cout << "," << room.tostring();
    }
    std::cout << "]";
}