#include "terra.h"

void scripted_preinsertion(istream &script) {
    char op[4];
    int x = 0, y = 0;

    while (script.peek() != EOF) {
        script >> op >> x >> y;

        switch (op[0]) {
            case 's':
                if (!mesh->is_used(x, y)) {
                    mesh->select(x, y);
                    mesh->is_used(x, y) = DATA_POINT_USED;
                }
                break;

            case 'i':
                if (!mesh->is_used(x, y))
                    mesh->is_used(x, y) = DATA_POINT_IGNORED;
                break;

            case 'u':
                if (!mesh->is_used(x, y))
                    mesh->is_used(x, y) = DATA_VALUE_UNKNOWN;
                break;

            default:
                break;
        }
    }
}

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

