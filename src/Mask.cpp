
#include <iostream>

#include "Mask.h"


RealMask *readMask(istream &in) {
    char magicP, magicNum = 0;

    int width, height = 0, maxval = 0;

    in >> magicP >> magicNum;
    in >> width >> height >> maxval;

    if (magicP != 'P') {
        cerr << "readMask: This is not PGM data." << endl;
        return nullptr;
    }

    auto *mask = new RealMask(width, height);

    if (magicNum == '2') {

        for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++) {
                double val;
                in >> val;
                mask->ref(i, j) = val;

            }
    } else if (magicNum == '5') {

        for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++) {

                unsigned char val;
                in >> val;
                mask->ref(i, j) = (double) val;
            }
    } else {
        cerr << "readMask: This is not PGM data." << endl;
        return nullptr;
    }


    auto max = (double) maxval;

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            mask->ref(i, j) /= max;

    return mask;
}
