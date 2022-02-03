#pragma once
#include <deque>
#include <string>
#include "item.hpp"

enum tile_type {
    tile_none=0,
    tile_wall,
    tile_floor,
    tile_bridge,
    tile_scaffold,

    tile_door,
    tile_locked_door,

    tile_stairs_up,
    tile_stairs_down,

    tile_grass,

    tile_water,
    tile_lava,

    tile_ore_gold,
    tile_ore_silver,
    tile_ore_copper,

    tile_magic_well,
    tile_magic_well_empty,

    tile_trap,
    tile_triggered_trap,

    tile_test_tile
};

class tile {
public:
    tile();
    tile(tile_type type);
    tile(tile_type type, int state);
    tile(tile_type type, bool hidden);
    tile(tile_type type, int state, bool hidden);
    ~tile();

    item heldItem;
    tile_type type;
    int state=0;
    bool hidden=false;

    std::string to_string();

    void reset();
    void reset(tile_type _type);
    void reset(int _state);
    void reset(bool _hidden);
    void reset(tile_type _type, int _state);
    void reset(int _state, bool _hidden);
    void reset(tile_type _type, bool _hidden);
    void reset(tile_type _type, int _state, bool _hidden);

    void onPlayerEnter();
};

typedef std::deque<std::deque<tile>> tileset;