#include <deque>
#include <string>
#include "tile.hpp"

tile::tile() : type(tile_type::tile_none) {};
tile::tile(tile_type _type) : type(_type) {};
tile::~tile() {};