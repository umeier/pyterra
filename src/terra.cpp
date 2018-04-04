//
// Created by uli on 04.04.18.
//

#include "terra.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;

PYBIND11_MODULE(pyterra, m) {
    m.def("read_double", &readDouble);
    m.def("greedy_insertion", &greedy_insertion);
}