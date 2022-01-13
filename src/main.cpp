#include "dungeon.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <time.h>

int main() {
    volatile int seed = time(NULL);
    dungeon_level d(seed);
}