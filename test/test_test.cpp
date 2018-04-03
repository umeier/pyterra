//
// Created by meier_ul on 03.04.18.
//

#define BOOST_TEST_MODULE My Test

#include <boost/test/included/unit_test.hpp>
#include "terra.h"

Mesh *mesh;
ImportMask *MASK;
double error_threshold = 0.0;
int point_limit = -1;


BOOST_AUTO_TEST_CASE(first_test) {
    int i = 1;
    BOOST_CHECK(i);
    BOOST_CHECK(i == 1);
}