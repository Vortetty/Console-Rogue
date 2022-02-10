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
ruleAutomata::ruleAutomata(vec2 gridSize, std::deque<std::deque<uint8_t>> grid): _size(gridSize), _grid(grid) {
    // Initialize to B3/S23 or the normal life rules, custom grid
    _birth = {3};
    _survival = {2, 3};
}
ruleAutomata::ruleAutomata(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival): _birth(birth), _survival(survival) {
    // Initialize to custom rules, 10x10 blank grid
    _size = {10, 10};
    _grid.resize(_size.y);
    for (int y = 0; y < _size.y; y++) {
        _grid[y].resize(_size.x);
        for (int x = 0; x < _size.x; x++) {
            _grid[y][x] = false;
        }
    }
}
ruleAutomata::ruleAutomata(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival, vec2 gridSize, std::deque<std::deque<uint8_t>> grid): _birth(birth), _survival(survival), _size(gridSize), _grid(grid) {
    // Initialize to custom rules, custom grid
}

void ruleAutomata::setRules(std::initializer_list<uint8_t> birth, std::initializer_list<uint8_t> survival) {
    this->_birth = birth;
    this->_survival = survival;
}
void ruleAutomata::setGrid(std::deque<std::deque<uint8_t>> grid) {
    this->_grid = grid;
}
void ruleAutomata::setCell(int x, int y, uint8_t alive) {
    _grid[y][x] = alive;
}
    
void ruleAutomata::fillGridRandomWithChance(int chance, PractRand::RNGs::Polymorphic::sfc64& rng) {
    for (int x = 0; x < _size.x; x++)
        for (int y = 0; y < _size.y; y++)
            _grid[x][y] = rng.raw64() % 100 < chance;
}
