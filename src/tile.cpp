#include <deque>
#include <string>
#include "tile.hpp"

tile::tile() : type(tile_type::tile_none) {};
tile::tile(tile_type _type) : type(_type) {};
tile::tile(tile_type _type, int _state) : type(_type), state(_state) {};
tile::~tile() {};

std::string tile::to_string() {
    switch (type) {
        case tile_type::tile_none: return " ";
        case tile_type::tile_wall: return "#";
        case tile_type::tile_floor: return ".";
        
        case tile_type::tile_door: return state%2 ? "│" : "─";
        case tile_type::tile_locked_door: return state%2 ? "║" : "═";
        case tile_type::tile_hidden_door: return state%2 ? "│" : "─";

        case tile_type::tile_stairs_up: return "⤒";
        case tile_type::tile_stairs_down: return "⤓";
    }
}