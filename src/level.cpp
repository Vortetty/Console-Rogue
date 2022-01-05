#include "dungeon.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "XoshiroCpp.hpp"
#include "box2d/box2d.h"
#include <deque>
#include <iostream>
#include <math.h>
#include <algorithm>

namespace xrand = XoshiroCpp;

dungeon_level::dungeon_level(uint64_t level) : levelId(level) {
    rng = xrand::Xoshiro256PlusPlus(level);
    generate();
}

dungeon_level::dungeon_level(dungeon& d, uint64_t level) : levelId(level) {
    rng = xrand::Xoshiro256PlusPlus(level);
    generate();
}

dungeon_level::~dungeon_level() {}

void dungeon_level::generate() {
    // https://www.gamedeveloper.com/programming/procedural-dungeon-generation-algorithm

    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    std::deque<rect> rooms;

    //std::cout << "[";
    for (int i=0; i<50; i++) {
        int w = rng() % 25 + 3;
        int h = rng() % 25 + 3;

        int x = -(w/2) + (int)(rng() % 50 - 25);
        int y = -(h/2) + (int)(rng() % 50 - 25);

        b2BodyDef bodydef;
        bodydef.position.Set(x, y);
        bodydef.type = b2_dynamicBody;
        bodydef.linearDamping = (float)0xffff;
        bodydef.linearVelocity = {rng() % 100 / (float)100, rng() % 100 / (float)100};
        
        b2Body* body = world.CreateBody(&bodydef);

        b2PolygonShape box;
        box.SetAsBox(h, w);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = (float)0xfffffffffffffff;
        fixtureDef.friction = 1000.0f;


        body->CreateFixture(&fixtureDef);
    }
    //std::cout << "]";

    float timeStep = 100.0f;
    uint32_t velocityIterations = 6;
    uint32_t positionIterations = 2;

    for (uint32_t i = 0; i < 60; ++i) {
        world.Step(timeStep, velocityIterations, positionIterations);
    }

    for (b2Body* b = world.GetBodyList(); b; b = b->GetNext()) {
        b2Vec2 pos = b->GetPosition();
        b2AABB aabb;
        b->GetFixtureList()->GetShape()->ComputeAABB(&aabb, b->GetTransform(), 0);
        rooms.push_back(rect{(int)pos.x, (int)pos.y, (int)aabb.upperBound.x - (int)aabb.lowerBound.x, (int)aabb.upperBound.y - (int)aabb.lowerBound.y});
    }

    std::cout << "[";
    std::cout << rooms[0].tostring();
    rooms.pop_front();
    for (rect room : rooms) {
        std::cout << "," << room.tostring();
    }
    std::cout << "]";
}