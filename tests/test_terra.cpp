//
// Created by meier_ul on 04.04.18.
//

#include <catch.hpp>
#include "terra.h"

Mesh *mesh;
Map *DEM;

using namespace std;


TEST_CASE("double_test") {
    vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    int width = 4;
    int height = 2;
    DEM = readDouble(data, width, height);
    mesh = new Mesh(DEM);
    REQUIRE(DEM->width == 4);
    REQUIRE(DEM->height == 2);
    REQUIRE(DEM->min == 1.0);
    REQUIRE(DEM->max == 8.0);
    REQUIRE(DEM->eval(0, 0) == 1.0);
    REQUIRE(DEM->eval(3, 1) == 8.0);
    REQUIRE(DEM->eval(3, 0) == 4.0);
    REQUIRE(DEM->eval(0, 1) == 5.0);
    mesh->greedy_insertion();
}

TEST_CASE("triangle_test") {
    vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
    int width = 4;
    int height = 3;
    DEM = readDouble(data, width, height);
    mesh = new Mesh(DEM, Vertex(0, 0), Vertex(0, 2), Vertex(3, 0));
    REQUIRE(mesh->eval(0, 0) == 1.0);
    REQUIRE(mesh->eval(0, 1) == 5.0);
    REQUIRE(mesh->eval(0, 2) == 9.0);
    REQUIRE(mesh->eval(3, 0) == 4.0);
    REQUIRE(mesh->is_used(3, 1) == 0);
    REQUIRE(DEM->min == 1.0);
    REQUIRE(DEM->max == 12.0);
    mesh->greedy_insertion();
}
