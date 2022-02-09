// cellular automata, take in ruleset and a grid of cells, or generate a random grid

#pragma once

#include <deque>
#include <initializer_list>
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"
#include "utils.hpp"

class ruleAutomata {
public:
    ruleAutomata(); // Initialize to B3/S23 or the normal life rules, 10x10 grid
    ruleAutomata(vec2 gridSize, std::deque<std::deque<uint8_t>> grid);
    ruleAutomata(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival);
    ruleAutomata(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival, vec2 gridSize, std::deque<std::deque<uint8_t>> grid);

    void setRules(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival);
    void setGrid(std::deque<std::deque<uint8_t>> grid);
    void setCell(int x, int y, uint8_t alive);
    
    void fillGridRandomWithChance(int chance, PractRand::RNGs::Polymorphic::sfc64& rng);
    
private:
    std::deque<uint8_t> birth = {3};
    std::deque<uint8_t> survival = {2, 3};
    std::deque<std::deque<uint8_t>> grid;
    vec2 size = {10, 10};
};
