#ifndef TERRA_MAP_H // -*- C++ -*-
#define TERRA_MAP_H

#include <cstdlib>
#include <iostream>

#include "Geom.h"

using namespace std;

class Map {
public:

    int width{};
    int height{};

    double min{}, max{};

    double operator()(int i, int j) { return eval(i, j); }

    double operator()(double i, double j) { return eval((int) i, (int) j); }

    double eval(double i, double j) { return eval((int) i, (int) j); }

    virtual double eval(int i, int j) = 0;

    virtual void rawRead(istream &) = 0;

    virtual void textRead(istream &) = 0;

    virtual void doubleRead(double[]) = 0;

    virtual void findLimits();
};

extern Map *readPGM(istream &);
extern Map *readDouble(double[], int width, int heigt);

template<class T>
class DirectMap : public Map {
    T *data;

protected:
    inline T &ref(int i, int j) {

        return data[j * width + i];
    }

public:

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

    DirectMap(int w, int h);

#pragma clang diagnostic pop

    double eval(int i, int j) override { return (double) ref(i, j); }

    void rawRead(istream &) override;

    void textRead(istream &) override;

    void doubleRead(double *) override;
};

typedef DirectMap<unsigned char> ByteMap;
typedef DirectMap<unsigned short> ShortMap;
typedef DirectMap<unsigned int> WordMap;
typedef DirectMap<double> DoubleMap;


#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

template<class T>
DirectMap<T>::DirectMap(int w, int h) {
    width = w;
    height = h;

    data = (T *) calloc((size_t) (w * h), sizeof(T)); // NOLINT
}

#pragma clang diagnostic pop

template<class T>
void DirectMap<T>::rawRead(istream &in) {
    auto *loc = (char *) data;
    int target = width * height * sizeof(T);

    while (target > 0) {
        in.read(loc, target);
        target -= in.gcount();
        loc += in.gcount();
    }
}

template<class T>
void DirectMap<T>::textRead(istream &in) {
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++) {
            double val;
            in >> val;
            ref(i, j) = (T) val;
        }
}

template<class T>
void DirectMap<T>::doubleRead(double *indata) {
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++) {
            double val;
            val = indata[j * width + i];
            ref(i, j) = (T) val;
        }
}

#endif
