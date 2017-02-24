#ifndef TERRA_VERTEX2_H // -*- C++ -*-
#define TERRA_VERTEX2_H

class Vertex2 {
protected:
    real elt[2];

    inline void copy(const Vertex2 &v);

public:
    // Standard constructors
    Vertex2(real x = 0, real y = 0) {
        elt[0] = x;
        elt[1] = y;
    }

    Vertex2(const Vertex2 &v) { copy(v); }

    Vertex2(const real *v) {
        elt[0] = v[0];
        elt[1] = v[1];
    }

    Vertex2 &clone() const { return *(new Vertex2(elt[0], elt[1])); }

    // Access methods
    real &operator()(int i) { return elt[i]; }

    const real &operator()(int i) const { return elt[i]; }

    real &operator[](int i) { return elt[i]; }

    const real &operator[](int i) const { return elt[i]; }

    // Assignment methods
    inline Vertex2 &operator=(const Vertex2 &v);

    inline Vertex2 &operator+=(const Vertex2 &v);

    inline Vertex2 &operator-=(const Vertex2 &v);

    inline Vertex2 &operator*=(real s);

    inline Vertex2 &operator/=(real s);

    // Arithmetic methods
    inline Vertex2 operator+(const Vertex2 &v) const;

    inline Vertex2 operator-(const Vertex2 &v) const;

    inline Vertex2 operator-() const;

    inline Vertex2 operator*(real s) const;

    inline Vertex2 operator/(real s) const;

    inline real operator*(const Vertex2 &v) const;

#ifdef _GLIBCXX_IOSTREAM

    // Input/Output methods
    friend std::ostream &operator<<(std::ostream &, const Vertex2 &);

    friend std::istream &operator>>(std::istream &, Vertex2 &);

#endif

    // Additional vector methods
    inline real length();

    inline real norm();

    inline real norm2();

    inline real unitize();

    inline int operator==(const Vertex2 &v) const {
        return (*this - v).norm2() < EPS2;
    }
};


inline void Vertex2::copy(const Vertex2 &v) {
    elt[0] = v.elt[0];
    elt[1] = v.elt[1];
}

inline Vertex2 &Vertex2::operator=(const Vertex2 &v) {
    copy(v);
    return *this;
}

inline Vertex2 &Vertex2::operator+=(const Vertex2 &v) {
    elt[0] += v[0];
    elt[1] += v[1];
    return *this;
}

inline Vertex2 &Vertex2::operator-=(const Vertex2 &v) {
    elt[0] -= v[0];
    elt[1] -= v[1];
    return *this;
}

inline Vertex2 &Vertex2::operator*=(real s) {
    elt[0] *= s;
    elt[1] *= s;
    return *this;
}

inline Vertex2 &Vertex2::operator/=(real s) {
    elt[0] /= s;
    elt[1] /= s;
    return *this;
}

///////////////////////

inline Vertex2 Vertex2::operator+(const Vertex2 &v) const {
    Vertex2 w(elt[0] + v[0], elt[1] + v[1]);
    return w;
}

inline Vertex2 Vertex2::operator-(const Vertex2 &v) const {
    Vertex2 w(elt[0] - v[0], elt[1] - v[1]);
    return w;
}

inline Vertex2 Vertex2::operator-() const {
    return Vertex2(-elt[0], -elt[1]);
}

inline Vertex2 Vertex2::operator*(real s) const {
    Vertex2 w(elt[0] * s, elt[1] * s);
    return w;
}

inline Vertex2 Vertex2::operator/(real s) const {
    Vertex2 w(elt[0] / s, elt[1] / s);
    return w;
}

inline real Vertex2::operator*(const Vertex2 &v) const {
    return elt[0] * v[0] + elt[1] * v[1];
}

inline real Vertex2::length() {
    return norm();
}

inline real Vertex2::norm() {
    return sqrt(elt[0] * elt[0] + elt[1] * elt[1]);
}

inline real Vertex2::norm2() {
    return elt[0] * elt[0] + elt[1] * elt[1];
}

inline real Vertex2::unitize() {
    real l = norm();
    if (l != 1.0)
        (*this) /= l;
    return l;
}

#ifdef _GLIBCXX_IOSTREAM

inline std::ostream &operator<<(std::ostream &out, const Vertex2 &v) {
    return out << (char *) "[" << v[0] << (char *) " " << v[1] << (char *) "]";
}

inline std::istream &operator>>(std::istream &in, Vertex2 &v) {
    return in >> (char *) "[" >> v[0] >> v[1] >> (char *) "]";
}

#endif


#endif
