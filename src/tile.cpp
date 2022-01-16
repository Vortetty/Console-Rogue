#include <deque>
#include <string>
#include "tile.hpp"

tile::tile() : type(tile_type::tile_none) {};
tile::tile(tile_type _type) : type(_type) {};
tile::tile(tile_type _type, int _state) : type(_type), state(_state) {};
tile::tile(tile_type _type, bool _hidden) : type(_type), hidden(_hidden) {};
tile::tile(tile_type _type, int _state, bool _hidden) : type(_type), state(_state), hidden(_hidden) {};
tile::~tile() {};

void tile::reset() {
    type = tile_type::tile_none;
    state = 0;
    hidden = false;
}
void tile::reset(tile_type _type) {
    type = _type;
    state = 0;
    hidden = false;
}
void tile::reset(int _state) {
    type = tile_type::tile_none;
    state = _state;
    hidden = false;
}
void tile::reset(bool _hidden) {
    type = tile_type::tile_none;
    state = 0;
    hidden = _hidden;
}
void tile::reset(tile_type _type, int _state) {
    type = _type;
    state = _state;
    hidden = false;
}
void tile::reset(int _state, bool _hidden) {
    type = tile_type::tile_none;
    state = _state;
    hidden = _hidden;
}
void tile::reset(tile_type _type, bool _hidden) {
    type = _type;
    state = 0;
    hidden = _hidden;
}
void tile::reset(tile_type _type, int _state, bool _hidden) {
    type = _type;
    state = _state;
    hidden = _hidden;
}


std::string tile::to_string() {
    switch (type) {
        case tile_type::tile_none: return " ";
        case tile_type::tile_wall: return "#";
        case tile_type::tile_floor: return ".";
        case tile_type::tile_bridge: return "=";
        case tile_type::tile_scaffold: return "_";
        
        case tile_type::tile_door:  return hidden ? "#" : (state%2 ? "│" : "─");
        case tile_type::tile_locked_door: return state%2 ? "║" : "═";

        case tile_type::tile_stairs_up: return "⤒";
        case tile_type::tile_stairs_down: return "⤓";

        case tile_type::tile_grass: return "╬";

        case tile_type::tile_water: return "≈";
        case tile_type::tile_lava: return "≈";

        case tile_type::tile_ore_gold: return "◸";
        case tile_type::tile_ore_silver: return "◹";
        case tile_type::tile_ore_copper: return "◺";

        case tile_type::tile_magic_well: return "◉";
        case tile_type::tile_magic_well_empty: return "◎";

        case tile_type::tile_trap: return hidden ? "." : "%";
        case tile_type::tile_triggered_trap: return "\\";

        case tile_type::tile_test_tile: return "█";
    }
}