#ifndef TERRA_SUBDIVISION_H // -*- C++ -*-
#define TERRA_SUBDIVISION_H

#include "Quadedge.h"

class Subdivision;

class Triangle : public Labelled {
    Edge *anchor{};
    Triangle *next_face{};

public:

    explicit Triangle(Edge *e, int t=0) : Labelled() {
	token = t;
	reshape(e);
    }

    Triangle *linkTo(Triangle *t) { next_face = t; return this; }
    Triangle *getLink() { return next_face; }
    Edge *getAnchor() { return anchor; }
    void dontAnchor(Edge *e);

    void reshape(Edge *e);

    virtual void update(Subdivision&); // called  to update stuff

    const Vertex& point1() const { return anchor->Org(); }
    const Vertex& point2() const { return anchor->Dest(); }
    const Vertex& point3() const { return anchor->Lprev()->Org(); }
};

typedef void (*edge_callback)(Edge *, void *);
typedef void (*face_callback)(Triangle&, void *);


class Subdivision {
private:
    Edge *startingEdge{};
    Triangle *first_face{};

protected:
    void initMesh(const Vertex&, const Vertex&, const Vertex&, const Vertex&);
    Subdivision() = default;

    Edge *makeEdge();
    Edge *makeEdge(Vertex& org, Vertex& dest);

    virtual Triangle *allocFace(Edge *e);
    Triangle& makeFace(Edge *e);


    void deleteEdge(Edge *);
    Edge *connect(Edge *a, Edge *b);
    void swap(Edge *e);

    //
    // Some random functions
    boolean ccwBoundary(const Edge *e);
    boolean onEdge(const Vertex&, Edge *);

public:

    Subdivision(Vertex& a, Vertex& b, Vertex& c, Vertex& d) { initMesh(a,b,c,d); }

    //
    // virtual functions for customization
    virtual boolean shouldSwap(const Vertex&, Edge *);


    boolean isInterior(Edge *);

    Edge *spoke(Vertex&, Edge *e);
    void optimize(Vertex&, Edge *);

    Edge *locate(const Vertex& x)  { return locate(x, startingEdge); }
    Edge *locate(const Vertex&, Edge *hint);
    Edge *insert(Vertex&, Triangle *t= nullptr);

    void overEdges(edge_callback, void *closure= nullptr);
    void overFaces(face_callback, void *closure= nullptr);
};


#ifdef _GLIBCXX_IOSTREAM
inline std::ostream& operator<<(std::ostream& out, Triangle& t)
{
    return out << "Triangle("<< t.point1() << " " << t.point2() << " "
	       << t.point3() << ")";
}
#endif


#endif
