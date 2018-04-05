#ifndef TERRA_TERRA_H // -*- C++ -*-
#define TERRA_TERRA_H

#include "GreedyInsert.h"
#include "Map.h"
#include "Mask.h"


extern Mesh *mesh;
extern Map *DEM;

extern double error_threshold;
extern int point_limit;

extern int goal_not_met();

extern void greedy_insertion();


#endif
