#pragma once

enum room_type {
    // Rooms every dungeon must have, these will always be in the 2 smallest rooms, which one gets the smaller one is random.
    room_stairs_up,          // Stairs to the level above
    room_stairs_down,        // Stairs to the level below

    // Rooms that aren't locked
    room_empty=0,            // empty room
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