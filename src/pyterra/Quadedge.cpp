#include <cstdlib>
#include <iostream>

#include "Quadedge.h"


using namespace std;

Edge::Edge(const Edge&) : Labelled() {
    cerr << "Edge: Edge assignments are forbidden." << endl;
    exit(1);
}


Edge::Edge(Edge *prev) : Labelled() {
    qprev = prev;
    prev->qnext = this;

    lface = nullptr;
    token = 0;
}

Edge::Edge() : Labelled() {
    Edge *e0 = this;
    auto *e1 = new Edge(e0);
    auto *e2 = new Edge(e1);
    auto *e3 = new Edge(e2);

    qprev = e3;
    e3->qnext = e0;

    e0->next = e0;
    e1->next = e3;
    e2->next = e2;
    e3->next = e1;

    lface = nullptr;
    token = 0;
}

Edge::~Edge()
{
    if( qnext )
    {
	Edge *e1 = qnext;
	Edge *e2 = qnext->qnext;
	Edge *e3 = qprev;

	e1->qnext = nullptr;
	e2->qnext = nullptr;
	e3->qnext = nullptr;

	delete e1;
	delete e2;
	delete e3;
    }
}


void splice(Edge *a, Edge *b)
{
    Edge *alpha = a->Onext()->Rot();
    Edge *beta  = b->Onext()->Rot();

    Edge *t1 = b->Onext();
    Edge *t2 = a->Onext();
    Edge *t3 = beta->Onext();
    Edge *t4 = alpha->Onext();

    a->next = t1;
    b->next = t2;
    alpha->next = t3;
    beta->next = t4;
}
