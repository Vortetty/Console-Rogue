#include <string>
#include "item.hpp"

item::item() {};
item::item(item_type type): type(type) {};
item::item(item_type type, int state): type(type), state(state) {};
item::~item() {};

std::string item::to_string() {};

void item::reset() {};
void item::reset(item_type _type) { type = _type; };
void item::reset(int _state) { state = _state; };
void item::reset(item_type _type, int _state) { type = _type; state = _state; };