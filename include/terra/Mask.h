#ifndef TERRA_MASK_H // -*- C++ -*-
#define TERRA_MASK_H

using namespace std;

class ImportMask {

public:
    int width, height;

    ImportMask() {
        width = 0;
        height = 0;
    }


    virtual double apply(int /*x*/, int /*y*/, double val) { return val; }
};


class RealMask : public ImportMask {
    double *data{};

public:
    RealMask(int w, int h);

    inline double &ref(int i, int j);

    double apply(int x, int y, double val) override { return ref(x, y) * val; }
};


inline RealMask::RealMask(int w, int h) {
    width = w;
    height = h;
    data = (double *) calloc((size_t) (w * h), sizeof(double));
}

inline double &RealMask::ref(int i, int j) {

    return data[j * width + i];
}

#endif
