#pragma once
#include <deque>
#include <string>

enum tile_type {
    tile_none=0,
    tile_wall,
    tile_floor,

    tile_door,
    tile_hidden_door,

    tile_stairs_up,
    tile_stairs_down,

    tile_grass,

    tile_water,
    tile_lava,

    tile_ore_gold,
    tile_ore_silver,
    tile_ore_copper,
    tile_ore_iron,

    tile_magic_well,
    tile_magic_well_empty,

    tile_trap,
    tile_triggered_trap
};

class tile {
public:
    tile();
    tile(tile_type type);
    tile(tile_type type, int state);
    ~tile();

    tile_type type;
    int state=0;
};

typedef std::deque<std::deque<tile>> tileset;