#include "dungeon.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "XoshiroCpp.hpp"
#include "delaunator.hpp"
#include <deque>
#include <vector>
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
    // Binary space partitioning
    // Minimum room size will be 7x7, and all cells must be 10x10, this ensures a possibility of varied rooms even at the smallest sizes.
    // Space to be partitioned will be 200x200
    const int min_room_size = 7;
    const int min_cell_size = 12;
    const int space_width = 200;
    const int space_height = 200;

    std::deque<rect> cells = utils::binarySpacePartition(rect{0, 0, space_width, space_height}, min_cell_size);
    std::deque<rect> rooms;

    for (auto& cell : cells) {
        int w = std::max((int)(rng() % cell.w), min_room_size);
        int h = std::max((int)(rng() % cell.h), min_room_size);

        int x = rng() % (cell.w - w) + cell.x;
        int y = rng() % (cell.h - h) + cell.y;


        rooms.push_back(
            rect { x, y, w, h }
        );
    }

    // Center points of rooms
    // x1, y1, x2, y2 ... xn, yn
    std::vector<double> delaunatorPoints;

    for (auto& room : rooms) {
        point center = room.center();
        delaunatorPoints.push_back(center.x);
        delaunatorPoints.push_back(center.y);
    }

    // Delaunator
    delaunator::Delaunator delaunated(delaunatorPoints);


    std::cout << "{" << std::endl;
    std::cout << "    \"cells\": [";
        std::cout << cells[0].tostring();
        cells.pop_front();
        for (rect cell : cells) {
            std::cout << "," << cell.tostring();
        }
    std::cout << "]," << std::endl;

    std::cout << "    \"rooms\": [";
        std::cout << rooms[0].tostring();
        rooms.pop_front();
        for (rect room : rooms) {
            std::cout << "," << room.tostring();
        }
    std::cout << "]," << std::endl;
    
    std::cout << "    \"triangles\": [";
        for(std::size_t i = 0; i < delaunated.triangles.size(); i+=3) {
            printf(
                "[[%f, %f], [%f, %f], [%f, %f]],",
                delaunated.coords[2 * delaunated.triangles[i]],        //tx0
                delaunated.coords[2 * delaunated.triangles[i] + 1],    //ty0
                delaunated.coords[2 * delaunated.triangles[i + 1]],    //tx1
                delaunated.coords[2 * delaunated.triangles[i + 1] + 1],//ty1
                delaunated.coords[2 * delaunated.triangles[i + 2]],    //tx2
                delaunated.coords[2 * delaunated.triangles[i + 2] + 1] //ty2
            );
        }
    std::cout << "]" << std::endl;
    std::cout << "}";
}