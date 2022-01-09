#pragma once
#include <deque>
#include <string>

enum tile_type {
    tile_none=0,
    tile_wall,
    tile_floor,
    tile_door
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