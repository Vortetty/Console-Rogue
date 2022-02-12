#include <deque>
#include <initializer_list>
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"
#include "utils.hpp"
#include "cellularAutomata.hpp"

ruleAutomata::ruleAutomata() {
    // Initialize to B3/S23 or the normal life rules, 10x10 grid
    _birth = {3};
    _survival = {2, 3};
    _size = {10, 10};
    _grid.resize(_size.x);
    for (int x = 0; x < _size.x; x++) {
        _grid[x].resize(_size.y);
        for (int y = 0; y < _size.y; y++) {
            _grid[x][y] = false;
        }
    }
}
ruleAutomata::ruleAutomata(automataGrid grid): _grid(grid.grid), _size(grid.size) {
    _birth = {3};
    _survival = {2, 3};
}
ruleAutomata::ruleAutomata(automataRule rule): _birth(rule.birth), _survival(rule.survival) {
    // 10x10 grid
    _size = {10, 10};
    _grid.resize(_size.x);
    for (int x = 0; x < _size.x; x++) {
        _grid[x].resize(_size.y);
        for (int y = 0; y < _size.y; y++) {
            _grid[x][y] = false;
        }
    }
}
ruleAutomata::ruleAutomata(automataRule rule, automataGrid grid): _birth(rule.birth), _survival(rule.survival), _grid(grid.grid), _size(grid.size) {}

void ruleAutomata::setRules(std::initializer_list<uint8_t> &birth, std::initializer_list<uint8_t> &survival) {
    this->_birth = birth;
    this->_survival = survival;
}
void ruleAutomata::setGrid(std::deque<std::deque<uint8_t>> &grid) {
    this->_grid = grid;
}
void ruleAutomata::setCell(int x, int y, uint8_t alive) {
    _grid[y][x] = alive;
}
void ruleAutomata::resize(vec2 newSize) {
    _size = newSize;
    _grid.resize(_size.x);
    for (int x = 0; x < _size.x; x++) {
        _grid[x].resize(_size.y);
        for (int y = 0; y < _size.y; y++) {
            _grid[x][y] = false;
        }
    }
}
automataGrid ruleAutomata::getGrid() {
    return automataGrid{_grid, _size};
};
automataRule ruleAutomata::getRule() {
    return automataRule{_birth, _survival};
}
    
void ruleAutomata::fillGridRandomWithChance(int chance, PractRand::RNGs::Polymorphic::sfc64& rng) {
    chance = chance % 101;
    for (int x = 0; x < _size.x; x++)
        for (int y = 0; y < _size.y; y++)
            _grid[x][y] = rng.raw64() % 100 < chance;
}

void ruleAutomata::simSteps(int steps) {
    if (disableEdgeSim) {
        for (int i = 0; i < steps; i++) {
            std::deque<std::deque<uint8_t>> oldGrid = _grid;

            for (int x = 1; x < _size.x+1; x++) {
                for (int y = 1; y < _size.y+1; y++) {
                    int count = 0;

                    count += (int)oldGrid[x][y-1];
                    count += (int)oldGrid[x][y+1];
                    count += (int)oldGrid[x-1][y];
                    count += (int)oldGrid[x+1][y];

                    count += (int)oldGrid[x-1][y-1];
                    count += (int)oldGrid[x+1][y-1];
                    count += (int)oldGrid[x-1][y+1];
                    count += (int)oldGrid[x+1][y+1];

                    if (std::find(_birth.begin(), _birth.end(), count) != _birth.end())
                        _grid[x][y] = true;
                    else if (std::find(_survival.begin(), _survival.end(), count) == _survival.end())
                        _grid[x][y] = false;
                }
            }
        }
    } else {
        for (int i = 0; i < steps; i++) {
            std::deque<std::deque<uint8_t>> oldGrid = _grid;

            for (int x = 0; x < _size.x; x++) {
                for (int y = 0; y < _size.y; y++) {
                    int count = 0;

                    if (x > 0)         count += (int)oldGrid[x-1][y]; else count += oobValue;
                    if (x < _size.x-1) count += (int)oldGrid[x+1][y]; else count += oobValue;
                    if (y > 0)         count += (int)oldGrid[x][y-1]; else count += oobValue;
                    if (y < _size.y-1) count += (int)oldGrid[x][y+1]; else count += oobValue;

                    if (x > 0 && y > 0)                 count += (int)oldGrid[x-1][y-1]; else count += oobValue;
                    if (x < _size.x-1 && y > 0)         count += (int)oldGrid[x+1][y-1]; else count += oobValue;
                    if (x > 0 && y < _size.y-1)         count += (int)oldGrid[x-1][y+1]; else count += oobValue;
                    if (x < _size.x-1 && y < _size.y-1) count += (int)oldGrid[x+1][y+1]; else count += oobValue;

                    if (std::find(_birth.begin(), _birth.end(), count) != _birth.end())
                        _grid[x][y] = true;
                    else if (std::find(_survival.begin(), _survival.end(), count) == _survival.end())
                        _grid[x][y] = false;
                }
            }
        }
    }
}
void ruleAutomata::simUntilStillLife(int max) {
    while (true) {
        std::deque<std::deque<uint8_t>> oldGrid = _grid;
        simSteps(1);
        if (max != -1 && max-- == 0)
            break;
        if (_grid == oldGrid)
            break;
    }
}

void ruleAutomata::setEdgeSimulationDisabled(bool disabled) {
    disableEdgeSim = disabled;
}
bool ruleAutomata::getEdgeSimulationDisabled() {
    return disableEdgeSim;
}

void ruleAutomata::setOOBValue(bool value) {
    oobValue = value;
}
bool ruleAutomata::getOOBValue() {
    return oobValue;
}
