#include <iostream>
#include <assert.h>
#include "GreedyInsert.h"

#include "Mask.h"

extern ImportMask *MASK;

void TrackedTriangle::update(Subdivision &s) {
    Mesh &gs = (Mesh &) s;
    gs.scanTriangle(*this);
}


Mesh::Mesh(Map *map) {
    H = map;
    heap = new Heap(128);

    int w = H->width;
    int h = H->height;

    is_used.init(w, h);
    int x, y;
    for (x = 0; x < w; x++)
        for (y = 0; y < h; y++)
            is_used(x, y) = DATA_POINT_UNUSED;


    initMesh(Vertex(0, 0),
             Vertex(0, h - 1),
             Vertex(w - 1, h - 1),
             Vertex(w - 1, 0));

    is_used(0, 0) = DATA_POINT_USED;
    is_used(0, h - 1) = DATA_POINT_USED;
    is_used(w - 1, h - 1) = DATA_POINT_USED;
    is_used(w - 1, 0) = DATA_POINT_USED;

    count = 4;
}


Triangle *Mesh::allocFace(Edge *e) {
    Triangle *t = new TrackedTriangle(e);

    heap->insert(t, -1.0);

    return t;
}


void Mesh::compute_plane(Plane &plane,
                         Triangle &T,
                         Map &map) {
    const Vertex &p1 = T.point1();
    const Vertex &p2 = T.point2();
    const Vertex &p3 = T.point3();

    Vec3 v1(p1, map(p1[X], p1[Y]));
    Vec3 v2(p2, map(p2[X], p2[Y]));
    Vec3 v3(p3, map(p3[X], p3[Y]));

    plane.init(v1, v2, v3);
}

///////////////////////////
//
// This is indeed an ugly hack.
// It should be replaced
//
static int vec2_y_compar(const void *a, const void *b) {
    Vertex &p1 = *(Vertex *) a,
            &p2 = *(Vertex *) b;

    return (p1[Y] == p2[Y]) ? 0 : (p1[Y] < p2[Y] ? -1 : 1);
}

static void order_triangle_points(Vertex *by_y,
                                  const Vertex &p1,
                                  const Vertex &p2,
                                  const Vertex &p3) {
    by_y[0] = p1;
    by_y[1] = p2;
    by_y[2] = p3;

    qsort(by_y, 3, sizeof(Vertex), vec2_y_compar);
}


void Mesh::scan_triangle_line(Plane &plane,
                              int y,
                              double x1, double x2,
                              Candidate &candidate) {
    int startx = (int) ceil(MIN(x1, x2));
    int endx = (int) floor(MAX(x1, x2));

    if (startx > endx) return;

    double z0 = plane(startx, y);
    double dz = plane.a;
    double z, diff;

    for (int x = startx; x <= endx; x++) {
        if (!is_used(x, y)) {
            z = H->eval(x, y);
            diff = fabs(z - z0);

            candidate.consider(x, y, MASK->apply(x, y, diff));
        }

        z0 += dz;
    }
}


void Mesh::scanTriangle(TrackedTriangle &T) {
    Plane z_plane;
    compute_plane(z_plane, T, *H);

    Vertex by_y[3];
    order_triangle_points(by_y, T.point1(), T.point2(), T.point3());
    Vertex &v0 = by_y[0];
    Vertex &v1 = by_y[1];
    Vertex &v2 = by_y[2];


    int y;
    int starty, endy;
    Candidate candidate;

    double dx1 = (v1[X] - v0[X]) / (v1[Y] - v0[Y]);
    double dx2 = (v2[X] - v0[X]) / (v2[Y] - v0[Y]);

    double x1 = v0[X];
    double x2 = v0[X];

    starty = (int) v0[Y];
    endy = (int) v1[Y];
    for (y = starty; y < endy; y++) {
        scan_triangle_line(z_plane, y, x1, x2, candidate);

        x1 += dx1;
        x2 += dx2;
    }

    /////////////////////////////

    dx1 = (v2[X] - v1[X]) / (v2[Y] - v1[Y]);
    x1 = v1[X];

    starty = (int) v1[Y];
    endy = (int) v2[Y];
    for (y = starty; y <= endy; y++) {
        scan_triangle_line(z_plane, y, x1, x2, candidate);

        x1 += dx1;
        x2 += dx2;
    }

    /////////////////////////////////
    //
    // We have now found the appropriate candidate point.
    //
    if (candidate.import < 1e-4) {
        if (T.token != NOT_IN_HEAP)
            heap->kill(T.token);

    } else {
        assert(!is_used(candidate.x, candidate.y));

        T.setCandidate(candidate.x, candidate.y, candidate.import);
        if (T.token == NOT_IN_HEAP)
            heap->insert(&T, candidate.import);
        else
            heap->update(&T, candidate.import);
    }
}

Edge *Mesh::select(int sx, int sy, Triangle *t) {
    if (is_used(sx, sy)) {
        cerr << "   WARNING: Tried to reinsert point: " << sx << " " << sy << endl;
        return NULL;
    }

    is_used(sx, sy) = DATA_POINT_USED;
    count++;
    Vertex v = Vertex(sx, sy);
    return insert(v, t);
}


int Mesh::greedyInsert() {
    heap_node *node = heap->extract();

    if (!node) return False;

    TrackedTriangle &T = *(TrackedTriangle *) node->obj;
    int sx, sy;
    T.getCandidate(&sx, &sy);

    select(sx, sy, &T);

    return True;
}

double Mesh::maxError() {
    heap_node *node = heap->top();

    if (!node)
        return 0.0;

    return node->import;
}

double Mesh::rmsError() {
    double err = 0.0;
    int width = H->width;
    int height = H->height;


    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            double diff = eval(i, j) - H->eval(i, j);
            err += diff * diff;
        }

    return sqrt(err / (width * height));
}


double Mesh::eval(int x, int y) {
    Vertex p(x, y);
    Triangle *T = locate(p)->Lface();

    Plane z_plane;
    compute_plane(z_plane, *T, *H);

    return z_plane(x, y);
}
