#pragma once
#include <string>

struct item_type {

};

class item {
public:
    item();
    item(item_type type);
    item(item_type type, int state);
    ~item();

    item_type type;
    int state=0;

    std::string to_string();

    void reset();
    void reset(item_type _type);
    void reset(int _state);
    void reset(item_type _type, int _state);
};