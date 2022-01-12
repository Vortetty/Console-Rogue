#pragma once
#include "dungeon.hpp"
#include "tile.hpp"
#include "room.hpp"
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"
#include <list>

class dungeon_level {
public:
    dungeon_level(uint64_t level);
    dungeon_level(dungeon& d, uint64_t level);
    dungeon_level(dungeon* d, uint64_t level);
    ~dungeon_level();

    void generate();

private:
    dungeon_level();

    uint64_t levelId; // Also acts as the seed for the level
    PractRand::RNGs::Polymorphic::sfc64 rng = PractRand::RNGs::Polymorphic::sfc64((PractRand::Uint64)0);
    dungeon* parentDungeon;

    std::deque<std::deque<tile>> tiles;
    std::deque<room> rooms;

    const int min_room_size = 7;
    const int min_cell_size = 12;
    const int space_width  = 200;
    const int space_height = 200;
};
