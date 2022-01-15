#pragma once
#include <map>
#include <memory>
#include "utils.hpp"
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"
#include "weightedDistribution.hpp"

enum room_type {
    // Rooms every dungeon must have, these will always be in the 2 smallest rooms, which one gets the smaller one is random0
    room_stairs_down=0,      // Stairs to the level below
    room_stairs_up,          // Stairs to the level above


    // Rooms that aren't locked
    room_empty,              // empty room
    room_maze,               // room containing a tiny maze with a random item
    room_bridges,            // room containing a bridge between all doors surrounded by void, and a random item separated from the bridge if possible
    room_chasm,              // room containing a large chasm with a random item in the center
    room_garden,             // room containing a garden

    room_fire_traps,         // room containing normal tiles, fire traps, hidden fire traps, and triggered traps
    room_ice_traps,          // room containing normal tiles, ice traps, hidden ice traps, and triggered traps
    room_poison_traps,       // room containing normal tiles, poison traps, hidden poison traps, and triggered traps
    room_lightning_traps,    // room containing normal tiles, lightning traps, hidden lightning traps, and triggered traps
    room_multi_traps,        // room containing all of the above

    room_wand,               // room containing a wand
    room_armor,              // room containing a piece of armor
    room_weapon,             // room containing a weapon
    room_multi_items,        // room containing a 2 of the above

    room_shop,               // room containing a shop, sells food, placable tiles(to fill voids), and 
    room_wand_forge,         // room containing a wand forge (sells random wand)
    room_armor_forge,        // room containing an armor forge (sells random armor)
    room_weapon_forge,       // room containing a weapon forge (sells random weapon)
    room_multi_forge,        // room containing a forge (sells one of each of the above)


    // Rooms that are locked
    room_locked_empty,       // empty room
    room_treasure,           // room where each tile has a 25% chance of containing gold between 1 and 10, and a 5% chance of containing 10-25
    room_wandry,             // room where each tile has a 10% chance of containing a wand
    room_armory,             // room where each tile has a 10% chance of containing armor of the same level as the player 
    room_weaponry,           // room where each tile has a 10% chance of containing a weapon of the same level as the player
    room_multi_items_locked, // room where each tile has a 10% chance of containing a wand, armor, or weapon of the same level as the player
    room_locked_traps        // A room filled with nothing but random traps on every tile
};

class room {
public:
    room(rect r, std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    room(rect r, std::deque<std::deque<tile>>& tiles, room_type type, PractRand::RNGs::Polymorphic::sfc64& rng);

    room_type type;
    rect room_rect;
    std::deque<point> doors;

private:
    void generate_stairs_up(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_stairs_down(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);

    void generate_empty(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_maze(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_bridges(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_chasm(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_garden(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);

    void generate_fire_traps(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_ice_traps(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_poison_traps(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_lightning_traps(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_multi_traps(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);

    void generate_wand(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_armor(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_weapon(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_multi_items(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);

    void generate_shop(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_wand_forge(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_armor_forge(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_weapon_forge(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_multi_forge(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);

    void generate_locked_empty(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_treasure(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_wandry(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_armory(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_weaponry(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_multi_items_locked(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);
    void generate_locked_traps(std::deque<std::deque<tile>>& tiles, PractRand::RNGs::Polymorphic::sfc64& rng);

    void generate_room(std::deque<std::deque<tile>>& tiles, room_type roomType, PractRand::RNGs::Polymorphic::sfc64& rng);

    inline static std::shared_ptr<std::deque<val_prob_pair<room_type>>> roomTypeWeights = std::make_shared<std::deque<val_prob_pair<room_type>>>(
        std::deque<val_prob_pair<room_type>>({
            {room_type::room_stairs_down, 0},
            {room_type::room_stairs_up, 0},

            {room_type::room_empty, 50},
            {room_type::room_maze, 25},
            {room_type::room_bridges, 25},
            {room_type::room_chasm, 25},
            {room_type::room_garden, 25},

            //{room_type::room_fire_traps, 25},
            //{room_type::room_ice_traps, 25},
            //{room_type::room_poison_traps, 25},
            //{room_type::room_lightning_traps, 25},
            //{room_type::room_multi_traps, 15},

            //{room_type::room_wand, 25},
            //{room_type::room_armor, 25},
            //{room_type::room_weapon, 25},
            //{room_type::room_multi_items, 15},

            //{room_type::room_shop, 25},
            //{room_type::room_wand_forge, 25},
            //{room_type::room_armor_forge, 25},
            //{room_type::room_weapon_forge, 25},
            //{room_type::room_multi_forge, 15},

            //{room_type::room_locked_empty, 1},
            //{room_type::room_treasure, 1},
            //{room_type::room_wandry, 1},
            //{room_type::room_armory, 1},
            //{room_type::room_weaponry, 1},
            //{room_type::room_multi_items_locked, 1},
            //{room_type::room_locked_traps, 1}
        })
    );

    inline static std::shared_ptr<weightedDistribution<room_type>> roomTypeGenerator = std::make_shared<weightedDistribution<room_type>>(*roomTypeWeights.get());
};
