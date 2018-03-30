#ifndef TERRA_QUADEDGE_H // -*- C++ -*-
#define TERRA_QUADEDGE_H

#include "Geom.h"

class Triangle;

class Edge : public Labelled {
private:
    Edge *qnext, *qprev;

    explicit Edge(Edge *prev);

protected:
    Vertex *data;
    Edge *next;

    Triangle *lface;

public:
    Edge();
    Edge(const Edge&);
    ~Edge();

    //
    // Primitive methods
    //
    Edge *Onext() const { return next; }
    Edge *Sym() const { return qnext->qnext; }
    Edge *Rot() const { return qnext; }
    Edge *invRot() const { return qprev; }

    //
    // Synthesized methods
    //
    Edge *Oprev() const { return Rot()->Onext()->Rot(); }
    Edge *Dnext() const { return Sym()->Onext()->Sym(); }
    Edge *Dprev() const { return invRot()->Onext()->invRot(); }

    Edge *Lnext() const { return invRot()->Onext()->Rot(); }
    Edge *Lprev() const { return Onext()->Sym(); }
    Edge *Rnext() const { return Rot()->Onext()->invRot(); }
    Edge *Rprev() const { return Sym()->Onext(); }


    Vertex& Org() const { return *data; }
    Vertex& Dest() const { return *Sym()->data; }

    Triangle *Lface() const { return lface; }
    void set_Lface(Triangle *t) { lface = t; }

    void set_end_points(Vertex &org, Vertex &dest)
    {
	data = &org;
	Sym()->data = &dest;
    }

    //
    // The fundamental topological operator
    friend void splice(Edge *a, Edge *b);
};


inline boolean rightOf(const Vertex& x, const Edge *e)
{
    return rightOf(x, e->Org(), e->Dest());
}

inline boolean leftOf(const Vertex& x, const Edge *e)
{
    return leftOf(x, e->Org(), e->Dest());
}


#ifdef _GLIBCXX_IOSTREAM
inline std::ostream& operator<<(std::ostream& out, const Edge *e)
{
    return out << "{ " << e->Org() << " ---> " << e->Dest() << " }";
}
#endif

#endif
