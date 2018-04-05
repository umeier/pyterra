#include "terra.h"

inline int goal_not_met() {
    return mesh->maxError() > error_threshold &&
           mesh->pointCount() < point_limit;
}


void greedy_insertion() {

    while (goal_not_met()) {
        if (!mesh->greedyInsert())
            break;
    }

}

