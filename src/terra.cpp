//
// Created by uli on 04.04.18.
//

#include "terra.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;
using namespace pybind11::literals;

typedef std::tuple<int, int> point;
typedef std::tuple<point, point, point> triangle;

Mesh *mesh;
double error_threshold = 0.0;
int point_limit = -1;
Map *DEM;


class PyTerra {

public:
    PyTerra(vector<double> &data, int width, int height);

    PyTerra(vector<double> &data, int width, int height, triangle tri);

    void setPointLimit(int);

    void setErrorThreshold(double);

    void do_insertion();

    vector<triangle3d> get_result();

};

PyTerra::PyTerra(vector<double> &data, int width, int height) {
    DEM = readDouble(data, width, height);
    mesh = new Mesh(DEM);
}

PyTerra::PyTerra(vector<double> &data, int width, int height, triangle tri) {
    point A = get<0>(tri);
    point B = get<1>(tri);
    point C = get<2>(tri);
    Vertex a = Vertex(get<0>(A), get<1>(A));
    Vertex b = Vertex(get<0>(B), get<1>(B));
    Vertex c = Vertex(get<0>(C), get<1>(C));
    DEM = readDouble(data, width, height);
    mesh = new Mesh(DEM, a, b, c);
}

void PyTerra::setPointLimit(int limit) {
    point_limit = limit;
}

void PyTerra::setErrorThreshold(double threshold) {
    error_threshold = threshold;
}

void PyTerra::do_insertion() {
    greedy_insertion();
}

vector<triangle3d> PyTerra::get_result() {
    calculated_tin calc_result;
    vector<Triangle *> triangles = mesh->getTriangles();
    for (auto it = triangles.begin(); it != triangles.end(); it++) {
        Triangle T = **it;
        const Vertex p1 = T.point1();
        const Vertex p2 = T.point2();
        const Vertex p3 = T.point3();
        point3d P1 = point3d(p1[X], p1[Y], DEM->eval(p1[X], p1[Y]));
        point3d P2 = point3d(p2[X], p2[Y], DEM->eval(p2[X], p2[Y]));
        point3d P3 = point3d(p3[X], p3[Y], DEM->eval(p3[X], p3[Y]));
        triangle3d tri = triangle3d(P1, P2, P3);
        calc_result.push_back(tri);
    }
    return calc_result;
}


PYBIND11_MODULE(pyterra, m) {
    m.doc() = "terra tin rendering plugin";
    py::class_<PyTerra> pyterra(m, "PyTerra");
    pyterra.def(py::init<vector<double> &, int, int>());
    pyterra.def(py::init<vector<double> &, int, int, triangle>());
    pyterra.def("set_point_limit", &PyTerra::setPointLimit);
    pyterra.def("set_error_threshold", &PyTerra::setErrorThreshold);
    pyterra.def("do_insertion", &PyTerra::do_insertion);
    pyterra.def("get_result", &PyTerra::get_result);
}