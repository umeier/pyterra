#include <cmath>

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

Map *readPGM(istream &in) {
    char magicP, magicNum;

    int width, height = 0, maxval = 0;

    in >> magicP;
    in >> magicNum;
    in >> width >> height >> maxval;

    if (magicP != 'P') {
        cerr << "readPGM: This is not PGM data." << endl;
        return nullptr;
    }

    Map *map;

    if (maxval < 256) {
        cerr << "readPGM: Allocating a ByteMap to hold data." << endl;
        map = new ByteMap(width, height);
    } else if (maxval < 65536) {
        cerr << "readPGM: Allocating a ShortMap to hold data." << endl;
        map = new ShortMap(width, height);
    } else {
        cerr << "readPGM: Allocating a WordMap to hold data." << endl;
        map = new WordMap(width, height);
    }

    switch (magicNum) {
        case '2':
            cerr << "readPGM: Looks like textual PGM data" << endl;
            map->textRead(in);
            break;

        case '5':
            cerr << "readPGM: Looks like binary PGM data" << endl;
            in.get(magicP); // read the EOL
            map->rawRead(in);
            break;

        default:
            cerr << "readPGM: This is not PGM data." << endl;
            return nullptr;
    }

    map->findLimits();

    return map;
}
