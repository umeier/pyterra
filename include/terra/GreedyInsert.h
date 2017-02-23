#ifndef TERRA_GREEDYINSERT_H // -*- C++ -*-
#define TERRA_GREEDYINSERT_H

#include "Heap.h"
#include "Subdivision.h"
#include "Map.h"

class TrackedTriangle : public Triangle {
    //
    // candidate position
    int sx, sy;


public:
    TrackedTriangle(Edge *e, int t = NOT_IN_HEAP)
            : Triangle(e, t) {
    }

    void update(Subdivision &);


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

    int x, y;
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

protected:

    Map *H;

    Triangle *allocFace(Edge *e);

    void compute_plane(Plane &, Triangle &, Map &);

    void scan_triangle_line(Plane &plane,
                            int y, double x1, double x2,
                            Candidate &candidate);

public:
    Mesh(Map *map);

    array2<char> is_used;

    Edge *select(int sx, int sy, Triangle *t = NULL);

    Map &getData() { return *H; }

    void scanTriangle(TrackedTriangle &t);

    int greedyInsert();

    unsigned int pointCount() { return count; }

    double maxError();

    double rmsError();

    double eval(int x, int y);
};

//
// These are the possible values of is_used(x,y):
#define DATA_POINT_UNUSED  0
#define DATA_POINT_USED    1
#define DATA_POINT_IGNORED 2
#define DATA_VALUE_UNKNOWN 3

#endif
