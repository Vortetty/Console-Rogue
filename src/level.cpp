#include "dungeon.hpp"
#include "level.hpp"
#include "utils.hpp"
#include "PractRand.h"
#include "PractRand/RNGs/sfc64.h"
#include "delaunator.hpp"
#include "kruskal.hpp"
#include <deque>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>

// Uncomment to enable diagonal paths, still experimental, generates some paths that dead-end
// #define ENABLE_DIAGONAL_PATH


dungeon_level::dungeon_level() {
    tiles = std::deque<std::deque<tile>>(space_width, std::deque<tile>(space_height, tile_type::tile_none));
    generate();
}

dungeon_level::dungeon_level(uint64_t level) : levelId(level) {
    rng.seed(levelId);
    tiles = std::deque<std::deque<tile>>(space_width, std::deque<tile>(space_height, tile_type::tile_none));
    generate();
}
dungeon_level::dungeon_level(dungeon& d, uint64_t level) : parentDungeon(&d), levelId(level) {
    rng.seed(levelId);
    tiles = std::deque<std::deque<tile>>(space_width, std::deque<tile>(space_height, tile_type::tile_none));
    generate();
}
dungeon_level::dungeon_level(dungeon* d, uint64_t level) : parentDungeon(d), levelId(level) {
    rng.seed(levelId);
    tiles = std::deque<std::deque<tile>>(space_width, std::deque<tile>(space_height, tile_type::tile_none));
    generate();
}
dungeon_level::~dungeon_level() {}

void dungeon_level::generate() {

    std::deque<rect> cells = utils::binarySpacePartition(rect{0, 0, space_width, space_height}, min_cell_size, rng);
    std::deque<rect> room_rects;

    for (auto& cell : cells) {
        int w = std::max((int)(rng.raw64() % cell.w), min_room_size);
        int h = std::max((int)(rng.raw64() % cell.h), min_room_size);

        int x = rng.raw64() % (cell.w - w) + cell.x;
        int y = rng.raw64() % (cell.h - h) + cell.y;


        room_rects.push_back(
            rect { x, y, w, h }
        );
    }

    // Center points of rooms
    // x1, y1, x2, y2 ... xn, yn
    std::vector<double> delaunatorPoints;

    for (rect r : room_rects) {
        point center = r.center();
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
    int tmpgoal = rng.raw64() % 8 + 3;
    while (tmp < tmpgoal) {
        int addTri = rng.raw64() % (delaunated.triangles.size()/3);

        int a = delaunated.triangles[addTri];
        int b = delaunated.triangles[addTri + 1];
        int c = delaunated.triangles[addTri + 2];

        int addEdge = rng.raw64() % 3;

        if (addEdge == 0) k.mst.push_back(Edge(a, b, 1));
        else if (addEdge == 1) k.mst.push_back(Edge(b, c, 1));
        else k.mst.push_back(Edge(c, a, 1));

        tmp++;
    }

    // Convert the edges in k.mst to a deque of rect_set
    std::deque<rect_set> mst;
    for (auto& edge : k.mst) {
        rect r1 = room_rects[edge.ss];
        rect r2 = room_rects[edge.dd];

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

        if (utils::overlap(r1.x, r1.x+r1.w, r2.x, r2.x+r2.w) > 0) {
            int beginX = std::max(r1.x, r2.x);
            int endX = std::min(r1.x+r1.w, r2.x+r2.w);

            int beginY = std::min(std::max(r1.y, r2.y), std::min(r1.y+r1.h, r2.y+r2.h));
            int endY = std::max(std::max(r1.y, r2.y), std::min(r1.y+r1.h, r2.y+r2.h));

            int pathX = rng.raw64() % (endX - beginX) + beginX;

            for (int y = beginY; y < endY; y++) {
                tiles[pathX][y] = tile(tile_type::tile_floor);
            }

        } else if (utils::overlap(r1.y, r1.y+r1.h, r2.y, r2.y+r2.h) > 0) {
            int beginY = std::max(r1.y, r2.y);
            int endY = std::min(r1.y+r1.h, r2.y+r2.h);

            int beginX = std::min(std::max(r1.x, r2.x), std::min(r1.x+r1.w, r2.x+r2.w));
            int endX = std::max(std::max(r1.x, r2.x), std::min(r1.x+r1.w, r2.x+r2.w));

            int pathY = rng.raw64() % (endY - beginY) + beginY;

            for (int x = beginX; x < endX; x++) {
                tiles[x][pathY] = tile(tile_type::tile_floor);
            }
        } else {
            // The corner of r1 that is closest to r2
            // 0 = top left
            // 1 = top right
            // 2 = bottom left
            // 3 = bottom right
            int corner = 0;

#ifdef ENABLE_DIAGONAL_PATH
            bool diagonal = rng.raw64() % 2 == 0;
#else
            bool diagonal = false;
#endif

            if      (r2.x < r1.x && r2.y < r1.y) corner = 0; 
            else if (r2.x > r1.x && r2.y < r1.y) corner = 1; 
            else if (r2.x < r1.x && r2.y > r1.y) corner = 2; 
            else if (r2.x > r1.x && r2.y > r1.y) corner = 3;

            line toDraw;

            switch (corner) {
                case 0: {
                    if (rng.raw64() % 2) { // r1 Top side, r2 Right side
                        toDraw.a.x = rng.raw64() % (r1.w-2-(2*diagonal)) + r1.x + 1 + diagonal;
                        toDraw.a.y = r1.y;

                        toDraw.b.x = r2.x + r2.w - 1;
                        toDraw.b.y = rng.raw64() % (r2.h-2-(2*diagonal)) + r2.y + 1 + diagonal;
                    } else { // r1 Left side, r2 Bottom side
                        toDraw.a.x = r1.x;
                        toDraw.a.y = rng.raw64() % (r1.h-2-(2*diagonal)) + r1.y + 1 + diagonal;

                        toDraw.b.x = rng.raw64() % (r2.w-2-(2*diagonal)) + r2.x + 1 + diagonal;
                        toDraw.b.y = r2.y + r2.h - 1;
                    }
                    break;
                }
                case 1: {
                    if (rng.raw64() % 2) { // r1 Top side, r2 Left side
                        toDraw.a.x = rng.raw64() % (r1.w-2-(2*diagonal)) + r1.x + 1 + diagonal;
                        toDraw.a.y = r1.y;

                        toDraw.b.x = r2.x + r2.w - 1;
                        toDraw.b.y = rng.raw64() % (r2.h-2-(2*diagonal)) + r2.y + 1 + diagonal;
                    } else { // r1 Right side, r2 Bottom side
                        toDraw.a.x = r1.x + r1.w - 1;
                        toDraw.a.y = rng.raw64() % (r1.h-2-(2*diagonal)) + r1.y + 1 + diagonal;

                        toDraw.b.x = rng.raw64() % (r2.w-2-(2*diagonal)) + r2.x + 1 + diagonal;
                        toDraw.b.y = r2.y + r2.h - 1;
                    }
                    break;
                }
                case 2: {
                    if (rng.raw64() % 2) { // r1 Bottom side, r2 Right side
                        toDraw.a.x = rng.raw64() % (r1.w-2-(2*diagonal)) + r1.x + 1 + diagonal;
                        toDraw.a.y = r1.y + r1.h - 1;

                        toDraw.b.x = r2.x + r2.w - 1;
                        toDraw.b.y = rng.raw64() % (r2.h-2-(2*diagonal)) + r2.y + 1 + diagonal;
                    } else { // r1 Left side, r2 Top side
                        toDraw.a.x = r1.x;
                        toDraw.a.y = rng.raw64() % (r1.h-2-(2*diagonal)) + r1.y + 1 + diagonal;

                        toDraw.b.x = rng.raw64() % (r2.w-2-(2*diagonal)) + r2.x + 1 + diagonal;
                        toDraw.b.y = r2.y;
                    }
                    break;
                }
                case 3: {
                    if (rng.raw64() % 2) { // r1 Bottom side, r2 Left side
                        toDraw.a.x = rng.raw64() % (r1.w-2-(2*diagonal)) + r1.x + 1 + diagonal;
                        toDraw.a.y = r1.y + r1.h - 1;

                        toDraw.b.x = r2.x;
                        toDraw.b.y = rng.raw64() % (r2.h-2-(2*diagonal)) + r2.y + 1 + diagonal;
                    } else { // r1 Right side, r2 Top side
                        toDraw.a.x = r1.x + r1.w - 1;
                        toDraw.a.y = rng.raw64() % (r1.h-2-(2*diagonal)) + r1.y + 1 + diagonal;

                        toDraw.b.x = rng.raw64() % (r2.w-2-(2*diagonal)) + r2.x + 1 + diagonal;
                        toDraw.b.y = r2.y;
                    }
                    break;
                }
            }
                    
            if (diagonal) toDraw.drawOnGrid(tiles, tile_type::tile_floor);
            else  toDraw.drawOnGridNoDiag(tiles, tile_type::tile_floor, rng.raw64() % 2);
        }
    }


    // Set all tile_type::tile_none that are adjacent(including diagonals) to a tile_type::tile_floor to tile_type::tile_wall
    for (int y = 0; y < space_height; y++) {
        for (int x = 0; x < space_width; x++) {
            if (tiles[x][y].type == tile_type::tile_none) {
                if      (x > 0 && tiles[x-1][y].type == tile_type::tile_floor)                                     tiles[x][y].reset(tile_type::tile_wall);
                else if (x < space_width-1 && tiles[x+1][y].type == tile_type::tile_floor)                         tiles[x][y].reset(tile_type::tile_wall);
                else if (y > 0 && tiles[x][y-1].type == tile_type::tile_floor)                                     tiles[x][y].reset(tile_type::tile_wall);
                else if (y < space_height-1 && tiles[x][y+1].type == tile_type::tile_floor)                        tiles[x][y].reset(tile_type::tile_wall);
                else if (x > 0 && y > 0 && tiles[x-1][y-1].type == tile_type::tile_floor)                          tiles[x][y].reset(tile_type::tile_wall);
                else if (x < space_width-1 && y > 0 && tiles[x+1][y-1].type == tile_type::tile_floor)              tiles[x][y].reset(tile_type::tile_wall);
                else if (x > 0 && y < space_height-1 && tiles[x-1][y+1].type == tile_type::tile_floor)             tiles[x][y].reset(tile_type::tile_wall);
                else if (x < space_width-1 && y < space_height-1 && tiles[x+1][y+1].type == tile_type::tile_floor) tiles[x][y].reset(tile_type::tile_wall);
            }
        }
    }


    std::sort (room_rects.begin(), room_rects.end(), [](const rect& a, const rect& b) {
        return a.w * a.h < b.w * b.h;
    });

    bool smallerRoomGetter = rng.raw64() % 2;
    rooms.push_back( room(room_rects[0], tiles, (room_type)smallerRoomGetter, rng) );
    rooms.push_back( room(room_rects[1], tiles, (room_type)!smallerRoomGetter, rng) );

    room_rects.pop_front();
    room_rects.pop_front();

    for (rect r : room_rects) {
        rooms.push_back(
            room(r, tiles, rng)
        );
    }


    //
    // Add key location generation, one key per locked door
    //

    for (int y = 0; y < space_width; y++) {
        for (int x = 0; x < space_height; x++) {
            std::cout << tiles[x][y].to_string() + " ";
        }
        std::cout << std::endl;
    }

}