#pragma once

typedef enum effect_type {
    effect_fire      = 1,
    effect_ice       = 1<<1,
    effect_poison    = 1<<2,
    effect_lightning = 1<<3
} effect_type;

inline uint8_t effectCount = 4;

typedef enum well_effect_type {
    well_effect_none    = 0,
    well_effect_heal    = 1,
    well_effect_cripple = 1<<1,
    well_effect_fire    = 1<<2
} well_effect_type;

inline uint8_t wellEffectCount = 4;
