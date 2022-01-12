#include "room.hpp"
#include "level.hpp"

room::room(rect r, PractRand::RNGs::Polymorphic::sfc64& rng) {
    roomTypeGenerator.sample(rng.raw64());
}

void room::generate_room(std::deque<std::deque<tile>> tiles, PractRand::RNGs::Polymorphic::sfc64& rng) {
    for (int x = room_rect.x; x < room_rect.x + room_rect.w; x++) {
        for (int y = room_rect.y; y < room_rect.y + room_rect.h; y++) {
            tiles[x][y] = tile(tile_type::tile_floor);
        }
    }

    // Set corners ahead of time to help with door generation
    tile& ct1 = tiles[room_rect.x - 1][room_rect.y - 1];
    tile& ct2 = tiles[room_rect.x + room_rect.w][room_rect.y - 1];
    tile& ct3 = tiles[room_rect.x - 1][room_rect.y + room_rect.h];
    tile& ct4 = tiles[room_rect.x + room_rect.w][room_rect.y + room_rect.h];

    if (ct1.type == tile_type::tile_floor) { ct1.type = tile_type::tile_door; ct1.state = 0; }
    else ct1.type = tile_type::tile_wall;
    if (ct2.type == tile_type::tile_floor) { ct2.type = tile_type::tile_door; ct2.state = 0; }
    else ct2.type = tile_type::tile_wall;
    if (ct3.type == tile_type::tile_floor) { ct3.type = tile_type::tile_door; ct3.state = 0; }
    else ct3.type = tile_type::tile_wall;
    if (ct4.type == tile_type::tile_floor) { ct4.type = tile_type::tile_door; ct4.state = 0; }
    else ct4.type = tile_type::tile_wall;

    for (int x = room_rect.x; x < room_rect.x + room_rect.w; x++) {
        tile& t1 = tiles[x][room_rect.y-1]; 
        tile& t2 = tiles[x][room_rect.y+room_rect.h];

        if (t1.type == tile_type::tile_floor && 
            tiles[x + 1][room_rect.y-1].type == tile_type::tile_wall && 
            tiles[x - 1][room_rect.y-1].type == tile_type::tile_wall) { t1.type = tile_type::tile_door; t1.state = 0; }
        else t1.type = tile_type::tile_wall;

        if (t2.type == tile_type::tile_floor && 
            tiles[x + 1][room_rect.y+room_rect.h].type == tile_type::tile_wall && 
            tiles[x - 1][room_rect.y+room_rect.h].type == tile_type::tile_wall) { t2.type = tile_type::tile_door; t2.state = 0; }
        else t2.type = tile_type::tile_wall;
    }
    for (int y = room_rect.y; y < room_rect.y + room_rect.h; y++) {
        tile& t1 = tiles[room_rect.x-1][y]; 
        tile& t2 = tiles[room_rect.x+room_rect.w][y];

        if (t1.type == tile_type::tile_floor && 
            tiles[room_rect.x-1][y + 1].type == tile_type::tile_wall && 
            tiles[room_rect.x-1][y - 1].type == tile_type::tile_wall) { t1.type = tile_type::tile_door; t1.state = 1; }
        else t1.type = tile_type::tile_wall;

        if (t2.type == tile_type::tile_floor && 
            tiles[room_rect.x+room_rect.w][y + 1].type == tile_type::tile_wall && 
            tiles[room_rect.x+room_rect.w][y - 1].type == tile_type::tile_wall) { t2.type = tile_type::tile_door; t2.state = 1; }
        else t2.type = tile_type::tile_wall;
    }

    if (ct1.type == tile_type::tile_none) ct1.type = tile_type::tile_wall;
    if (ct2.type == tile_type::tile_none) ct2.type = tile_type::tile_wall;
    if (ct3.type == tile_type::tile_none) ct3.type = tile_type::tile_wall;
    if (ct4.type == tile_type::tile_none) ct4.type = tile_type::tile_wall;

    switch (roomTypeGenerator.sample(rng.raw64())) {
        case room_type::room_stairs_up: return generate_stairs_up(tiles, rng);
        case room_type::room_stairs_down: return generate_stairs_down(tiles, rng);

        case room_type::room_empty: return generate_empty(tiles, rng);
        case room_type::room_maze: return generate_maze(tiles, rng);
        case room_type::room_bridges: return generate_bridges(tiles, rng);
        case room_type::room_chasm: return generate_chasm(tiles, rng);
        case room_type::room_garden: return generate_garden(tiles, rng);

        case room_type::room_fire_traps: return generate_fire_traps(tiles, rng);
        case room_type::room_ice_traps: return generate_ice_traps(tiles, rng);
        case room_type::room_poison_traps: return generate_poison_traps(tiles, rng);
        case room_type::room_lightning_traps: return generate_lightning_traps(tiles, rng);
        case room_type::room_multi_traps: return generate_multi_traps(tiles, rng);

        case room_type::room_wand: return generate_wand(tiles, rng);
        case room_type::room_armor: return generate_armor(tiles, rng);
        case room_type::room_weapon: return generate_weapon(tiles, rng);
        case room_type::room_multi_items: return generate_multi_items(tiles, rng);

        case room_type::room_shop: return generate_shop(tiles, rng);
        case room_type::room_wand_forge: return generate_wand_forge(tiles, rng);
        case room_type::room_armor_forge: return generate_armor_forge(tiles, rng);
        case room_type::room_weapon_forge: return generate_weapon_forge(tiles, rng);
        case room_type::room_multi_forge: return generate_multi_forge(tiles, rng);

        case room_type::room_locked_empty: return generate_locked_empty(tiles, rng);
        case room_type::room_treasure: return generate_treasure(tiles, rng);
        case room_type::room_wandry: return generate_wandry(tiles, rng);
        case room_type::room_armory: return generate_armory(tiles, rng);
        case room_type::room_weaponry: return generate_weaponry(tiles, rng);
        case room_type::room_multi_items_locked: return generate_multi_items_locked(tiles, rng);
        case room_type::room_locked_traps: return generate_locked_traps(tiles, rng);

        default: return generate_empty(tiles, rng);
    }
}

void room::generate_stairs_up(std::deque<std::deque<tile>> tiles, PractRand::RNGs::Polymorphic::sfc64& rng) {
    point p = room_rect.center();
    tiles[p.x][p.y].type = tile_type::tile_stairs_up;
}

void room::generate_stairs_down(std::deque<std::deque<tile>> tiles, PractRand::RNGs::Polymorphic::sfc64& rng) {
    point p = room_rect.center();
    tiles[p.x][p.y].type = tile_type::tile_stairs_down;
}

void room::generate_empty(std::deque<std::deque<tile>> tiles, PractRand::RNGs::Polymorphic::sfc64& rng) {
    // Nothing
    // Add enemies some day
}

void room::generate_maze(std::deque<std::deque<tile>> tiles, PractRand::RNGs::Polymorphic::sfc64& rng) {
    // Generate a maze, to be implemented
}