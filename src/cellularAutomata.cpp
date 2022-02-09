// cellular automata, take in ruleset and a grid of cells, or generate a random grid

#pragma once

#include <deque>
#include <initializer_list>
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"
#include "utils.hpp"
#include "cellularAutomata.hpp"

ruleAutomata::ruleAutomata() {
    // Initialize to B3/S23 or the normal life rules, 10x10 grid
    birth = {3};
    survival = {2, 3};
    size = {10, 10};
    grid.resize(size.y);
    for (int y = 0; y < size.y; y++) {
        grid[y].resize(size.x);
        for (int x = 0; x < size.x; x++) {
            grid[y][x] = false;
        }
    }
}
ruleAutomata::ruleAutomata(vec2 gridSize, std::deque<std::deque<bool>> grid): size(gridSize), grid(grid) {
    // Initialize to B3/S23 or the normal life rules, custom grid
    birth = {3};
    survival = {2, 3};
}
ruleAutomata::ruleAutomata(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival): birth(birth), survival(survival) {
    // Initialize to custom rules, 10x10 blank grid
    size = {10, 10};
    grid.resize(size.y);
    for (int y = 0; y < size.y; y++) {
        grid[y].resize(size.x);
        for (int x = 0; x < size.x; x++) {
            grid[y][x] = false;
        }
    }
}
ruleAutomata::ruleAutomata(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival, vec2 gridSize, std::deque<std::deque<bool>> grid): birth(birth), survival(survival), size(gridSize), grid(grid) {
    // Initialize to custom rules, custom grid
}

void ruleAutomata::setRules(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival);
void ruleAutomata::setGrid(std::deque<std::deque<bool>> grid);
void ruleAutomata::setCell(int x, int y, bool alive);
    
void ruleAutomata::fillGridRandomWithChance(int chance, PractRand::RNGs::Polymorphic::sfc64& rng);
