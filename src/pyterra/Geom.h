#ifndef TERRA_GEOM_H // -*- C++ -*-
#define TERRA_GEOM_H

////////////////////////////////////////////////////////////////////////
//
// Define some basic types and values
//
////////////////////////////////////////////////////////////////////////


#define EPS 1e-6
#define EPS2 (EPS*EPS)

typedef int boolean;

enum Axis {
    X, Y, Z
};

#include <cmath>
#include "Vertex.h"
#include "Vertex3.h"

#ifndef NULL
#define NULL 0
#endif

#ifndef True
#define True 1
#define False 0
#endif

class Labelled {
public:
    int token;
};




////////////////////////////////////////////////////////////////////////
//
// Here we define some useful geometric functions
//
////////////////////////////////////////////////////////////////////////

//
// triArea returns TWICE the area of the oriented triangle ABC.
// The area is positive when ABC is oriented counterclockwise.
inline double triArea(const Vertex &a, const Vertex &b, const Vertex &c) {
    return (b[X] - a[X]) * (c[Y] - a[Y]) - (b[Y] - a[Y]) * (c[X] - a[X]);
}

inline boolean ccw(const Vertex &a, const Vertex &b, const Vertex &c) {
    return triArea(a, b, c) > 0;
}

inline boolean rightOf(const Vertex &x, const Vertex &org, const Vertex &dest) {
    return ccw(x, dest, org);
}

inline boolean leftOf(const Vertex &x, const Vertex &org, const Vertex &dest) {
    return ccw(x, org, dest);
}

// Returns True if the point d is inside the circle defined by the
// points a, b, c. See Guibas and Stolfi (1985) p.107.
//
inline boolean inCircle(const Vertex &a, const Vertex &b, const Vertex &c,
                        const Vertex &d) {
    return (a[0] * a[0] + a[1] * a[1]) * triArea(b, c, d) -
           (b[0] * b[0] + b[1] * b[1]) * triArea(a, c, d) +
           (c[0] * c[0] + c[1] * c[1]) * triArea(a, b, d) -
           (d[0] * d[0] + d[1] * d[1]) * triArea(a, b, c) > EPS;
}


class Plane {
public:

    double a{}, b{}, c{};

    Plane() = default;

    inline void init(const Vertex3 &p, const Vertex3 &q, const Vertex3 &r);

    double operator()(double x, double y) { return a * x + b * y + c; }

    double operator()(int x, int y) { return a * x + b * y + c; }
};

inline void Plane::init(const Vertex3 &p, const Vertex3 &q, const Vertex3 &r)
// find the plane z=ax+by+c passing through three points p,q,r
{
    // We explicitly declare these (rather than putting them in a
    // Vector) so that they can be allocated into registers.
    double ux = q[X] - p[X], uy = q[Y] - p[Y], uz = q[Z] - p[Z];
    double vx = r[X] - p[X], vy = r[Y] - p[Y], vz = r[Z] - p[Z];
    double den = ux * vy - uy * vx;

    a = (uz * vy - uy * vz) / den;
    b = (ux * vz - uz * vx) / den;
    c = p[Z] - a * p[X] - b * p[Y];
}


class Line {

private:
    double a, b, c;

public:
    Line(const Vertex &p, const Vertex &q) {
        Vertex t = q - p;
        double l = t.length();
        a = t[Y] / l;
        b = -t[X] / l;
        c = -(a * p[X] + b * p[Y]);
    }

    inline double eval(const Vertex &p) const {
        return (a * p[X] + b * p[Y] + c);
    }

};


#endif
