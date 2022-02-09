// cellular automata, take in ruleset and a grid of cells, or generate a random grid

#pragma once

#include <deque>
#include <initializer_list>
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"

class ruleAutomata {
public:
    ruleAutomata(); // Initialize to B3/S23 or the normal life rules, 10x10 grid
    ruleAutomata(std::deque<std::deque<bool>> grid);
    ruleAutomata(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival);
    ruleAutomata(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival, std::deque<std::deque<bool>> grid);

    void setRules(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival);
private:
    std::deque<uint8_t> birth = {};
    std::deque<uint8_t> survival = {};
    std::deque<std::deque<uint8_t>> grid = {};
};
