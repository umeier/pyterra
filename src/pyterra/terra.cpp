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


class PyTerra {

    Mesh *mesh = nullptr;
    Map *DEM = nullptr;

public:
    PyTerra(vector<double> &data, int width, int height);

    PyTerra(vector<double> &data, int width, int height, triangle tri);

    ~PyTerra();

    void setPointLimit(int);

    void setErrorThreshold(double);

    void do_insertion();

    int getPointCount();

    double getError();

    vector<triangle3d> get_result();

    vector<point3d> get_points();

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
    mesh->setPointLimit(limit);
}

void PyTerra::setErrorThreshold(double threshold) {
    mesh->setErrorThreshold(threshold);
}

void PyTerra::do_insertion() {
    mesh->greedy_insertion();
}

vector<triangle3d> PyTerra::get_result() {
    calculated_tin calc_result;
    vector<Triangle *> triangles = mesh->getTriangles();
    for (auto &triangle : triangles) {
        Triangle T = *triangle;
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

double PyTerra::getError() {
    return mesh->maxError();
}

int PyTerra::getPointCount() {
    return mesh->pointCount();
}

vector<point3d> PyTerra::get_points() {
    vector<point3d> result;
    int width = DEM->width;
    int height = DEM->height;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (mesh->is_used(x, y) == DATA_POINT_USED) {
                point3d p = point3d(x, y, DEM->eval(x, y));
                result.push_back(p);
            }
        }
    }
    return result;
}

PyTerra::~PyTerra() {
    DEM = nullptr;
    mesh = nullptr;
}


PYBIND11_MODULE(pyterra, m) {
    m.doc() = "pyterra tin rendering plugin";
    py::class_<PyTerra>(m, "PyTerra")
    .def(py::init<vector<double> &, int, int>())
    .def(py::init<vector<double> &, int, int, triangle>())
    .def("set_point_limit", &PyTerra::setPointLimit)
    .def("set_error_threshold", &PyTerra::setErrorThreshold)
    .def("do_insertion", &PyTerra::do_insertion)
    .def("get_result", &PyTerra::get_result)
    .def("get_error", &PyTerra::getError)
    .def("get_point_count", &PyTerra::getPointCount)
    .def("get_points", &PyTerra::get_points);
}