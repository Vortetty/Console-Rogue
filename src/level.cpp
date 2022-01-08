#include "dungeon.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "XoshiroCpp.hpp"
#include "delaunator.hpp"
#include "kruskal.hpp"
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

    // Run kruskal on delaunator output 
    Graph g;

    for (int i = 0; i < delaunated.triangles.size(); i += 3) {
        int a = delaunated.triangles[i];
        int b = delaunated.triangles[i + 1];
        int c = delaunated.triangles[i + 2];

        int x1 = delaunatorPoints[2 * a];
        int y1 = delaunatorPoints[2 * a + 1];
        int x2 = delaunatorPoints[2 * b];
        int y2 = delaunatorPoints[2 * b + 1];
        int x3 = delaunatorPoints[2 * c];
        int y3 = delaunatorPoints[2 * c + 1];

        g.addEdge(a, b, utils::distance(point{x1, y1}, point{x2, y2}));
        g.addEdge(b, c, utils::distance(point{x2, y2}, point{x3, y3}));
        g.addEdge(c, a, utils::distance(point{x3, y3}, point{x1, y1}));
    }

    Kruskal k(delaunatorPoints.size());
    k.createMST(g);

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
                "[[%i, %i], [%i, %i], [%i, %i]],",
                (int)delaunated.coords[2 * delaunated.triangles[i]],        //tx0
                (int)delaunated.coords[2 * delaunated.triangles[i] + 1],    //ty0
                (int)delaunated.coords[2 * delaunated.triangles[i + 1]],    //tx1
                (int)delaunated.coords[2 * delaunated.triangles[i + 1] + 1],//ty1
                (int)delaunated.coords[2 * delaunated.triangles[i + 2]],    //tx2
                (int)delaunated.coords[2 * delaunated.triangles[i + 2] + 1] //ty2
            );
        }
    std::cout << "]," << std::endl;

    std::cout << "    \"edges\": [";
        for(Edge& edge : k.mst) {
            printf(
                "[[%i, %i], [%i, %i]],",
                (int)delaunatorPoints[2 * edge.ss],
                (int)delaunatorPoints[2 * edge.ss + 1],
                (int)delaunatorPoints[2 * edge.dd],
                (int)delaunatorPoints[2 * edge.dd + 1]
            );
        }
    std::cout << "]" << std::endl;
    std::cout << "}";
}