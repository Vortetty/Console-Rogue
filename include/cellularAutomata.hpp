// cellular automata, take in ruleset and a grid of cells, or generate a random grid

#pragma once

#include <deque>
#include <initializer_list>
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"
#include "utils.hpp"

struct automataGrid { 
    std::deque<std::deque<uint8_t>> &grid;
    vec2 size = {10, 10};
};

struct automataRule { // Defaults to life
    std::deque<uint8_t> birth = {3};
    std::deque<uint8_t> survival = {2, 3};
};

class ruleAutomata {
public:
    ruleAutomata(); // Initialize to B3/S23 or the normal life rules, 10x10 grid
    ruleAutomata(automataGrid grid);
    ruleAutomata(automataRule rule);
    ruleAutomata(automataRule rule, automataGrid grid);

    void setRules(std::initializer_list<uint8_t> &birth, std::initializer_list<uint8_t> &survival);
    void setGrid(std::deque<std::deque<uint8_t>> &grid);
    void setCell(int x, int y, uint8_t alive);
    void resize(vec2 newSize);
    automataGrid getGrid();
    automataRule getRule();
    
    void fillGridRandomWithChance(int chance, PractRand::RNGs::Polymorphic::sfc64& rng);

    void simSteps(int steps);
    void simUntilStillLife(int max = -1); // May run forever if there is no stable state and max is -1

    void setEdgeSimulationDisabled(bool disabled);
    bool getEdgeSimulationDisabled();

    void setOOBValue(bool value);
    bool getOOBValue();
    
private:
    std::deque<uint8_t> _birth = {3};
    std::deque<uint8_t> _survival = {2, 3};
    std::deque<std::deque<uint8_t>> _grid;
    vec2 _size = {10, 10};
    bool disableEdgeSim = false;
    bool oobValue = 0;
};
