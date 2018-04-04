#include <cstdlib>
#include <iostream>

#include "Subdivision.h"

using namespace std;

Edge *Subdivision::makeEdge(Vertex &org, Vertex &dest) {
    auto *e = new Edge();
    e->set_end_points(org, dest);
    return e;
}

Edge *Subdivision::makeEdge() {
    return new Edge();
}

void Subdivision::initMesh(const Vertex &A, const Vertex &B,
                           const Vertex &C, const Vertex &D) {
    Vertex &a = A.clone();
    Vertex &b = B.clone();
    Vertex &c = C.clone();
    Vertex &d = D.clone();

    Edge *ea = makeEdge();
    ea->set_end_points(a, b);

    Edge *eb = makeEdge();
    splice(ea->Sym(), eb);
    eb->set_end_points(b, c);

    Edge *ec = makeEdge();
    splice(eb->Sym(), ec);
    ec->set_end_points(c, d);

    Edge *ed = makeEdge();
    splice(ec->Sym(), ed);
    ed->set_end_points(d, a);
    splice(ed->Sym(), ea);

    Edge *diag = makeEdge();
    splice(ed->Sym(), diag);
    splice(eb->Sym(), diag->Sym());
    diag->set_end_points(a, c);

    startingEdge = ea;

    first_face = nullptr;

    makeFace(ea->Sym()).update(*this);
    makeFace(ec->Sym()).update(*this);

}


void Subdivision::deleteEdge(Edge *e) {
    splice(e, e->Oprev());
    splice(e->Sym(), e->Sym()->Oprev());

    delete e;
}

Edge *Subdivision::connect(Edge *a, Edge *b) {
    Edge *e = makeEdge();
    splice(e, a->Lnext());
    splice(e->Sym(), b);
    e->set_end_points(a->Dest(), b->Org());

    return e;
}

void Subdivision::swap(Edge *e) {
    Triangle *f1 = e->Lface();
    Triangle *f2 = e->Sym()->Lface();

    Edge *a = e->Oprev();
    Edge *b = e->Sym()->Oprev();

    splice(e, a);
    splice(e->Sym(), b);
    splice(e, a->Lnext());
    splice(e->Sym(), b->Lnext());
    e->set_end_points(a->Dest(), b->Dest());


    f1->reshape(e);
    f2->reshape(e->Sym());
}



//
// Subdivision iterators
//

static unsigned int timestamp = 0;

static void overEdge(Edge *e, edge_callback fn, void *closure) {
    if (e->token != timestamp) {
        e->token = timestamp;
        e->Sym()->token = timestamp;

        (*fn)(e, closure);

        overEdge(e->Onext(), fn, closure);
        overEdge(e->Oprev(), fn, closure);
        overEdge(e->Dnext(), fn, closure);
        overEdge(e->Dprev(), fn, closure);
    }
}

void Subdivision::overEdges(edge_callback fn, void *closure) {
    if (++timestamp == 0)
        timestamp = 1;

    overEdge(startingEdge, fn, closure);
}

void Subdivision::overFaces(face_callback fn, void *closure) {
    Triangle *t = first_face;

    while (t) {
        (*fn)(*t, closure);
        t = t->getLink();
    }
}




//
// Random predicates
//

boolean Subdivision::ccwBoundary(const Edge *e) {
    return !rightOf(e->Oprev()->Dest(), e);
}


boolean Subdivision::onEdge(const Vertex &x, Edge *e) {
    double t1, t2, t3;

    t1 = (x - e->Org()).length();
    t2 = (x - e->Dest()).length();

    if (t1 < EPS || t2 < EPS)
        return True;

    t3 = (e->Org() - e->Dest()).length();

    if (t1 > t3 || t2 > t3)
        return False;

    Line line(e->Org(), e->Dest());
    return (fabs(line.eval(x)) < EPS);
}


boolean Subdivision::isInterior(Edge *e)
//
// Tests whether e is an interior edge.
// 
// WARNING: This topological test will not work if the boundary is
//          a triangle.  This is not a problem here; the boundary is
//          always a rectangle.  But if you try to adapt this code, please
//          keep this in mind.
{
    return (e->Lnext()->Lnext()->Lnext() == e &&
            e->Rnext()->Rnext()->Rnext() == e);
}

boolean Subdivision::shouldSwap(const Vertex &x, Edge *e) {
    Edge *t = e->Oprev();
    return inCircle(e->Org(), t->Dest(), e->Dest(), x);
}


Edge *Subdivision::locate(const Vertex &x, Edge *start) {
    Edge *e = start;
    double t = triArea(x, e->Dest(), e->Org());

    if (t > 0) {                  // x is to the right of edge e
        t = -t;
        e = e->Sym();
    }


    while (True) {
        Edge *eo = e->Onext();
        Edge *ed = e->Dprev();

        double to = triArea(x, eo->Dest(), eo->Org());
        double td = triArea(x, ed->Dest(), ed->Org());

        if (td > 0)                       // x is below ed
            if (to > 0 || (to == 0 && t == 0)) {// x is interior, or origin endpoint
                startingEdge = e;
                return e;
            } else {                      // x is below ed, below eo
                t = to;
                e = eo;
            }
        else                            // x is on or above ed
        if (to > 0)                   // x is above eo
            if (td == 0 && t == 0) {    // x is destination endpoint
                startingEdge = e;
                return e;
            } else {                  // x is on or above ed and above eo
                t = td;
                e = ed;
            }
        else                        // x is on or below eo
        if (t == 0 && !leftOf(eo->Dest(), e))
            // x on e but subdiv. is to right
            e = e->Sym();
        else if (random() & 1) {  // x is on or above ed and
            t = to;             // on or below eo; step randomly
            e = eo;
        } else {
            t = td;
            e = ed;
        }
    }
}


Edge *Subdivision::spoke(Vertex &x, Edge *e) {
    Triangle *new_faces[4];
    int facedex = 0;

    //
    // NOTE: e is the edge returned by locate(x)
    //

    if ((x == e->Org()) || (x == e->Dest())) {
        // point is already in the mesh
        //
        cerr << "WARNING: Tried to reinsert point: " << x << endl;
        cerr << "         org: " << e->Org() << endl;
        cerr << "        dest: " << e->Dest() << endl;
        return nullptr;
    }

    Edge *boundary_edge = nullptr;

    Triangle *lface = e->Lface();
    lface->dontAnchor(e);
    new_faces[facedex++] = lface;

    if (onEdge(x, e)) {
        if (ccwBoundary(e)) {
            //
            // e lies on the boundary
            // Defer deletion until after new edges are added.
            boundary_edge = e;
        } else {
            Triangle *sym_lface = e->Sym()->Lface();
            new_faces[facedex++] = sym_lface;
            sym_lface->dontAnchor(e->Sym());


            e = e->Oprev();
            deleteEdge(e->Onext());
        }
    } else {
        // x lies within the Lface of e
    }

    Edge *base = makeEdge(e->Org(), x.clone());

    splice(base, e);

    startingEdge = base;
    do {
        base = connect(e, base->Sym());
        e = base->Oprev();
    } while (e->Lnext() != startingEdge);

    if (boundary_edge)
        deleteEdge(boundary_edge);

    // Update all the faces in our new spoked polygon.
    // If point x on perimeter, then don't add an exterior face


    base = boundary_edge ? startingEdge->Rprev() : startingEdge->Sym();
    do {
        if (facedex)
            new_faces[--facedex]->reshape(base);
        else
            makeFace(base);

        base = base->Onext();

    } while (base != startingEdge->Sym());

    return startingEdge;
}


//
// s is a spoke pointing OUT from x
//
void Subdivision::optimize(Vertex &x, Edge *s) {
    Edge *start_spoke = s;
    Edge *spoke = s;

    do {

        Edge *e = spoke->Lnext();
        Edge *t = e->Oprev();

        if (isInterior(e) && shouldSwap(x, e))
            swap(e);
        else {
            spoke = spoke->Onext();
            if (spoke == start_spoke)
                break;
        }

    } while (True);

    //
    // Now, update all the triangles

    spoke = start_spoke;

    do {
        Edge *e = spoke->Lnext();
        Triangle *t = e->Lface();

        if (t) t->update(*this);

        spoke = spoke->Onext();
    } while (spoke != start_spoke);
}

Edge *Subdivision::insert(Vertex &x, Triangle *tri) {
    Edge *e = tri ? locate(x, tri->getAnchor()) : locate(x);

    Edge *start_spoke = spoke(x, e);

    if (start_spoke)
        optimize(x, start_spoke->Sym());

    return start_spoke;
}


Triangle *Subdivision::allocFace(Edge *e) {
    return new Triangle(e);
}

Triangle &Subdivision::makeFace(Edge *e) {
    Triangle *t = allocFace(e);

    first_face = t->linkTo(first_face);

    return *t;
}

void Subdivision::initMesh(const Vertex &A, const Vertex &B, const Vertex &C) {

    Vertex &a = A.clone();
    Vertex &b = B.clone();
    Vertex &c = C.clone();

    Edge *ea = makeEdge();
    ea->set_end_points(a, b);

    Edge *eb = makeEdge();
    splice(ea->Sym(), eb);
    eb->set_end_points(b, c);

    Edge *ec = makeEdge();
    splice(eb->Sym(), ec);
    ec->set_end_points(c, a);
    splice(ec->Sym(), ea);

    startingEdge = ea;

    first_face = nullptr;

    makeFace(ea->Sym()).update(*this);
}


void Triangle::dontAnchor(Edge *e) {
    if (anchor == e) {
        anchor = e->Lnext();
    }
}

void Triangle::reshape(Edge *e) {
    anchor = e;

    e->set_Lface(this);
    e->Lnext()->set_Lface(this);
    e->Lprev()->set_Lface(this);

}


void Triangle::update(Subdivision &)
// called by reshape to update stuff
//
// the default method will do nothing
{


}
