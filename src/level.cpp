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

    //b2PolygonShape leftWallBox, rightWallBox, topWallBox, bottomWallBox;
    //leftWallBox.SetAsBox(500.0f, 10.0f);
    //rightWallBox.SetAsBox(500.0f, 10.0f);
    //topWallBox.SetAsBox(10.0f, 500.0f);
    //bottomWallBox.SetAsBox(10.0f, 500.0f);

    //b2BodyDef leftwalldef;
    //leftwalldef.position.Set(-500, -500);
    //b2Body* leftwallbody = world.CreateBody(&leftwalldef);
    //leftwallbody->CreateFixture(&leftWallBox, 0.0f);

    //b2BodyDef topwalldef;
    //topwalldef.position.Set(-500, -500);
    //b2Body* topwallbody = world.CreateBody(&topwalldef);
    //topwallbody->CreateFixture(&topWallBox, 0.0f);

    //b2BodyDef rightwalldef;
    //rightwalldef.position.Set(-500, 500);
    //b2Body* rightwallbody = world.CreateBody(&rightwalldef);
    //rightwallbody->CreateFixture(&rightWallBox, 0.0f);

    //b2BodyDef bottomwalldef;
    //bottomwalldef.position.Set(500, -500);
    //b2Body* bottomwallbody = world.CreateBody(&bottomwalldef);
    //bottomwallbody->CreateFixture(&bottomWallBox, 0.0f);

    //std::cout << "[";
    for (int i=0; i<50; i++) {
        int w = rng() % 25 + 3;
        int h = rng() % 25 + 3;

        int x = -(w/2) + (int)(rng() % 10 - 5);
        int y = -(h/2) + (int)(rng() % 10 - 5);

        b2BodyDef bodydef;
        bodydef.position.Set(x, y);
        bodydef.type = b2_dynamicBody;
        //bodydef.linearDamping = 0.1f;
        //bodydef.linearVelocity = {rng() % 100 / (float)100, rng() % 100 / (float)100};
        
        b2Body* body = world.CreateBody(&bodydef);
        body->SetAngularVelocity(rng() % 2 == 1 ? 0.1 : -0.1);

        b2PolygonShape box;
        box.SetAsBox(h, w);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 1.0f;

        body->CreateFixture(&fixtureDef);
    }
    //std::cout << "]";

    float timeStep = 1.0f;
    uint32_t velocityIterations = 6;
    uint32_t positionIterations = 2;

    for (uint32_t i = 0; i < 3000; ++i) {
        world.Step(timeStep, velocityIterations, positionIterations);
    }

    // Apply gravity towards 0,0
    for (int i=0; i < 5000; i++) {
        for (b2Body* b = world.GetBodyList(); b; b = b->GetNext()) {
            b2Vec2 pos = b->GetPosition();
            b2Vec2 vel = b->GetLinearVelocity();
            float force = 9.8f / pow(pow(pos.x, 2) + pow(pos.y, 2), 2);

            // Apply gravitational force towards center
            b->ApplyForceToCenter(b2Vec2(force * (0-pos.x)/(0-pos.y), force * (1-(0-pos.x)/(0-pos.y))), true);
        }
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