#ifndef TERRA_GREEDYINSERT_H // -*- C++ -*-
#define TERRA_GREEDYINSERT_H

#include "Heap.h"
#include "Subdivision.h"
#include "Map.h"
#include "Mask.h"

typedef vector<Triangle *> tin_triangles;


typedef tuple<int, int, double> point3d;
typedef tuple<point3d, point3d, point3d> triangle3d;
typedef vector<triangle3d> calculated_tin;

class TrackedTriangle : public Triangle {
    //
    // candidate position
    int sx{}, sy{};


public:
    explicit TrackedTriangle(Edge *e, int t = NOT_IN_HEAP)
            : Triangle(e, t) {
    }

    void update(Subdivision &) override;


    void setCandidate(int x, int y, double) {
        sx = x;
        sy = y;
    }

    void getCandidate(int *x, int *y) {
        *x = sx;
        *y = sy;
    }
};


class Candidate {
public:

    int x{}, y{};
    double import;

    Candidate() { import = -HUGE; }

    void consider(int sx, int sy, double i) {
        if (i > import) {
            x = sx;
            y = sy;
            import = i;
        }
    }
};


class Mesh : public Subdivision {
    Heap *heap;
    unsigned int count;
    ImportMask *MASK;

protected:

    Map *H;

    Triangle *allocFace(Edge *e) override;

    void compute_plane(Plane &, Triangle &, Map &);

    void scan_triangle_line(Plane &plane,
                            int y, double x1, double x2,
                            Candidate &candidate);

public:
    explicit Mesh(Map *map);
    Mesh(Map *map, const Vertex &, const Vertex &, const Vertex &);

    array2<char> is_used;

    Edge *select(int sx, int sy, Triangle *t = nullptr);

    void scanTriangle(TrackedTriangle &t);

    int greedyInsert();

    unsigned int pointCount() { return count; }

    double maxError();

    double eval(int x, int y);

    tin_triangles getTriangles();
};

//
// These are the possible values of is_used(x,y):
#define DATA_POINT_UNUSED  0
#define DATA_POINT_USED    1
#define DATA_POINT_IGNORED 2
#define DATA_VALUE_UNKNOWN 3

#endif
