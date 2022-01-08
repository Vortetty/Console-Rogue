#pragma once
#include "dungeon.hpp"
#include "tile.hpp"
#include "XoshiroCpp.hpp"
#include <list>

namespace xrand = XoshiroCpp;

class dungeon_level {
public:
    dungeon_level(uint64_t level);
    dungeon_level(dungeon& d, uint64_t level);
    ~dungeon_level();

    void generate();

private:
    uint64_t levelId; // Also acts as the seed for the level
    xrand::Xoshiro256PlusPlus rng;

    tile tiles[200][200];
};
