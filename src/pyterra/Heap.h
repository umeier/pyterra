#ifndef TERRA_HEAP_H // -*- C++ -*-
#define TERRA_HEAP_H

#include "Geom.h"
#include "Array.h"

#define NOT_IN_HEAP (-47)

//
//
// This file extracted from ~/anim/lab/mlab
//
//

class heap_node {
public:
    double import;
    Labelled *obj;

    heap_node() { obj= nullptr; import=0.0; }

    heap_node(const heap_node& h) { import=h.import; obj=h.obj; }
};



class Heap : public array<heap_node> {

    //
    // The actual size of the heap.  array::length()
    // simply returns the amount of allocated space
    int size;

    void swap(int i, int j);

    int parent(int i) { return (i-1)/2; }
    int left(int i) { return 2*i+1; }
    int right(int i) { return 2*i+2; }

    void upheap(int i);
    void downheap(int i);

public:

    Heap() { size=0; }

    explicit Heap(int s) : array<heap_node>(s) { size=0; }


    void insert(Labelled *, double);

    void update(Labelled *, double);

    heap_node *extract();
    heap_node *top() { return size<1 ? (heap_node *) nullptr : &ref(0); }
    heap_node *kill(int i);
};



#endif
