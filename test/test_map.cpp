//
// Created by meier_ul on 04.04.18.
//

#define BOOST_TEST_MODULE test_map

#include <boost/test/included/unit_test.hpp>
#include "terra.h"
#include "test_settings.h"


Mesh *mesh;
double error_threshold = 0.0;
int point_limit = -1;
Map *DEM;

BOOST_AUTO_TEST_SUITE(terra_test_suite)


    BOOST_AUTO_TEST_CASE(double_test) {
        vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
        int width = 4;
        int height = 2;
        DEM = readDouble(data, width, height);
        mesh = new Mesh(DEM);
        BOOST_CHECK(DEM->width == 4);
        BOOST_CHECK(DEM->height == 2);
        BOOST_CHECK(DEM->min == 1.0);
        BOOST_CHECK(DEM->max == 8.0);
        BOOST_CHECK(DEM->eval(0, 0) == 1.0);
        BOOST_CHECK(DEM->eval(3, 1) == 8.0);
        BOOST_CHECK(DEM->eval(3, 0) == 4.0);
        BOOST_CHECK(DEM->eval(0, 1) == 5.0);
        greedy_insertion();
    }

    BOOST_AUTO_TEST_CASE(triangle_test) {
        vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
        int width = 4;
        int height = 3;
        DEM = readDouble(data, width, height);
        mesh = new Mesh(DEM, Vertex(0, 0), Vertex(0, 2), Vertex(3, 0));
        BOOST_CHECK(mesh->eval(0, 0) == 1.0);
        BOOST_CHECK(mesh->eval(0, 1) == 5.0);
        BOOST_CHECK(mesh->eval(0, 2) == 9.0);
        BOOST_CHECK(mesh->eval(3, 0) == 4.0);
        BOOST_CHECK(mesh->is_used(3, 1) == 0);
        BOOST_CHECK(DEM->min == 1.0);
        BOOST_CHECK(DEM->max == 12.0);
        greedy_insertion();
    }

BOOST_AUTO_TEST_SUITE_END()