#include <cmath>
#include <utility>
#include "Geom.h"
#include "Map.h"

void Map::findLimits() {
    min = HUGE;
    max = -HUGE;

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            double val = eval(i, j);

            if (val < min) min = val;
            if (val > max) max = val;

        }
}

Map *readDouble(vector<double> values, int width, int height) {
    Map *map;
    map = new DoubleMap(width, height);
    map->doubleRead(std::move(values));
    map->findLimits();
    return map;
}


