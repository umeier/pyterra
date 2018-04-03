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

static void announce_goal() {
    cerr << "Goal conditions met:" << endl;
    cerr << "     error=" << mesh->maxError()
         << " [thresh=" << error_threshold << "]" << endl;
    cerr << "     points=" << mesh->pointCount()
         << " [limit=" << point_limit << "]" << endl;
}

void greedy_insertion() {

    while (goal_not_met()) {
        if (!mesh->greedyInsert())
            break;
    }

    announce_goal();
}

