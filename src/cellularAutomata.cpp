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
    _grid.resize(_size.y);
    for (int y = 0; y < _size.y; y++) {
        _grid[y].resize(_size.x);
        for (int x = 0; x < _size.x; x++) {
            _grid[y][x] = false;
        }
    }
}
ruleAutomata::ruleAutomata(automataGrid &grid): _grid(grid.grid), _size(grid.size) {
    _birth = {3};
    _survival = {2, 3};
}
ruleAutomata::ruleAutomata(automataRule &rule): _birth(rule.birth), _survival(rule.survival) {
    // 10x10 grid
    _size = {10, 10};
    _grid.resize(_size.y);
    for (int y = 0; y < _size.y; y++) {
        _grid[y].resize(_size.x);
        for (int x = 0; x < _size.x; x++) {
            _grid[y][x] = false;
        }
    }
}
ruleAutomata::ruleAutomata(automataRule &rule, automataGrid &grid): _birth(rule.birth), _survival(rule.survival), _grid(grid.grid), _size(grid.size) {}
ruleAutomata::ruleAutomata(vec2 &gridSize, std::deque<std::deque<uint8_t>> &grid): _size(gridSize), _grid(grid) {
    // Initialize to B3/S23 or the normal life rules
    _birth = {3};
    _survival = {2, 3};
}
ruleAutomata::ruleAutomata(std::initializer_list<uint8_t> &birth, std::initializer_list<uint8_t> &survival): _birth(birth), _survival(survival) {
    // 10x10 grid
    _size = {10, 10};
    _grid.resize(_size.y);
    for (int y = 0; y < _size.y; y++) {
        _grid[y].resize(_size.x);
        for (int x = 0; x < _size.x; x++) {
            _grid[y][x] = false;
        }
    }
}
ruleAutomata::ruleAutomata(automataRule &rule, vec2 &gridSize, std::deque<std::deque<uint8_t>> &grid): _birth(rule.birth), _survival(rule.survival), _size(gridSize), _grid(grid) {}
ruleAutomata::ruleAutomata(std::initializer_list<uint8_t> &birth, std::initializer_list<uint8_t> &survival, automataGrid &grid): _birth(birth), _survival(survival), _grid(grid.grid), _size(grid.size) {}
ruleAutomata::ruleAutomata(std::initializer_list<uint8_t> &birth, std::initializer_list<uint8_t> &survival, vec2 &gridSize, std::deque<std::deque<uint8_t>> &grid): _birth(birth), _survival(survival), _size(gridSize), _grid(grid) {}

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
automataGrid ruleAutomata::getGrid() {
    return automataGrid{_grid, _size};
};
automataRule ruleAutomata::getRule() {
    return automataRule{_birth, _survival};
}
void ruleAutomata::getGrid(automataGrid *grid) {
    grid->grid = _grid;
    grid->size = _size;
}
void ruleAutomata::getRule(automataRule *rule) {
    rule->birth = _birth;
    rule->survival = _survival;
}
void ruleAutomata::getGrid(automataGrid &grid) {
    grid.grid = _grid;
    grid.size = _size;
}
void ruleAutomata::getRule(automataRule &rule) {
    rule.birth = _birth;
    rule.survival = _survival;
}
    
void ruleAutomata::fillGridRandomWithChance(int chance, PractRand::RNGs::Polymorphic::sfc64& rng) {
    chance = chance % 101;
    for (int x = 0; x < _size.x; x++)
        for (int y = 0; y < _size.y; y++)
            _grid[x][y] = rng.raw64() % 100 < chance;
}

void ruleAutomata::simStep() {
    std::deque<std::deque<uint8_t>> newGrid;
    newGrid.resize(_size.y);
    for (int y = 0; y < _size.y; y++) {
        newGrid[y].resize(_size.x);
        for (int x = 0; x < _size.x; x++) {
            newGrid[y][x] = _grid[y][x];
        }
    }

    for (int y = 0; y < _size.y; y++) {
        for (int x = 0; x < _size.x; x++) {
            int aliveNeighbors = 0;
            for (int yy = -1; yy <= 1; yy++) {
                for (int xx = -1; xx <= 1; xx++) {
                    if (yy == 0 && xx == 0) continue;
                    if (x + xx < 0 || x + xx >= _size.x || y + yy < 0 || y + yy >= _size.y) continue;
                    if (_grid[y + yy][x + xx]) aliveNeighbors++;
                }
            }
            if (_grid[y][x]) {
                if (aliveNeighbors < _survival.front() || aliveNeighbors > _survival.back()) newGrid[y][x] = false;
            } else {
                if (aliveNeighbors >= _birth.front() && aliveNeighbors <= _birth.back()) newGrid[y][x] = true;
            }
        }
    }
    _grid = newGrid;
}
void ruleAutomata::simSteps(int steps) {
    for (int i = 0; i < steps; i++) {
        simStep();
    }
}

void ruleAutomata::setEdgeSimulationDisabled(bool disabled) {
    disableEdgeSim = disabled;
}
bool ruleAutomata::getEdgeSimulationDisabled() {
    return disableEdgeSim;
}
void ruleAutomata::getEdgeSimulationDisabled(bool *disabled) {
    *disabled = disableEdgeSim;
}
void ruleAutomata::getEdgeSimulationDisabled(bool &disabled) {
    disabled = disableEdgeSim;
}