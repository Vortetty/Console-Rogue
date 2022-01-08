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

    int tmp = 0;
    int tmpgoal = rng() % 8 + 3;
    while (tmp < tmpgoal) {
        int addTri = rng() % (delaunated.triangles.size()/3);

        int a = delaunated.triangles[addTri];
        int b = delaunated.triangles[addTri + 1];
        int c = delaunated.triangles[addTri + 2];

        int addEdge = rng() % 3;

        if (addEdge == 0) k.mst.push_back(Edge(a, b, 1));
        else if (addEdge == 1) k.mst.push_back(Edge(b, c, 1));
        else k.mst.push_back(Edge(c, a, 1));

        tmp++;
    }

    // Convert the edges in k.mst to a deque of rect_set
    std::deque<rect_set> mst;
    for (auto& edge : k.mst) {
        rect r1 = rooms[edge.ss];
        rect r2 = rooms[edge.dd];

        mst.push_back(rect_set{r1, r2});
    }

    // Determine the best way to create a path to the specified goal
    // This is done by determining if a room's x overlaps with another room's x by at least three tiles
    // If it does, then the room is considered to be above or below and a path will be drawn vertically
    // If it doesn't, then you determine if the room's y overlaps with another room's y by at least three tiles
    // If it does, then the room is considered to be to the left or right and a path will be drawn horizontally
    // If neither is true, a path will be made either diagonally, or with a vertical and horizontal line
    for (rect_set rs : mst) {
        rect r1 = rs.r1;
        rect r2 = rs.r2;

        if (r1.x+r1.w > r2.x && r2.x+r2.w > r1.x) {

            int beginX = std::max(r1.x, r2.x);
            int endX = std::min(r1.x+r1.w, r2.x+r2.w);

            int beginY = std::max(r1.y, r2.y);
            int endY = std::min(r1.y+r1.h, r2.y+r2.h);

            int pathX = beginX + (endX - beginX) / 2;

            for (int y = beginY; y < endY; y++) {
                tiles[pathX][y] = tile(tile_type::tile_floor);
            }

        } else if (r1.y+r1.h > r2.y && r2.y+r2.h > r1.y) {

            int beginX = std::max(r1.x, r2.x);
            int endX = std::min(r1.x+r1.w, r2.x+r2.w);

            int beginY = std::max(r1.y, r2.y);
            int endY = std::min(r1.y+r1.h, r2.y+r2.h);

            int pathY = beginY + (endY - beginY) / 2;

            for (int x = beginX; x < endX; x++) {
                tiles[x][pathY] = tile(tile_type::tile_floor);
            }
        } else {
            std::cout << "Diagonal path" << std::endl;
        }
    }

    for (rect room : rooms) {
        for (int x = room.x; x < room.x + room.w; x++) {
            for (int y = room.y; y < room.y + room.h; y++) {
                tiles[x][y] = tile(tile_type::tile_floor);
            }
        }
    }

    for (int x = 0; x < 200; x++) {
        for (int y = 0; y < 200; y++) {
            std::cout << (tiles[x][y].type == tile_type::tile_floor ? "." : " ");
        }
        std::cout << std::endl;
    }
}