#ifndef TERRA_VERTEX3_H // -*- C++ -*-
#define TERRA_VERTEX3_H

class Vertex3 {
protected:
    real elt[3];

    inline void copy(const Vertex3 &v);

public:
    // Standard constructors
    Vertex3(real x = 0, real y = 0, real z = 0) {
        elt[0] = x;
        elt[1] = y;
        elt[2] = z;
    }

    Vertex3(const Vertex2 &v, real z) {
        elt[0] = v[0];
        elt[1] = v[1];
        elt[2] = z;
    }

    Vertex3(const Vertex3 &v) { copy(v); }

    Vertex3(const real *v) {
        elt[0] = v[0];
        elt[1] = v[1];
        elt[2] = v[2];
    }

    // Access methods
    real &operator()(int i) { return elt[i]; }

    const real &operator()(int i) const { return elt[i]; }

    real &operator[](int i) { return elt[i]; }

    const real &operator[](int i) const { return elt[i]; }

    // Assignment methods
    inline Vertex3 &operator=(const Vertex3 &v);

    inline Vertex3 &operator+=(const Vertex3 &v);

    inline Vertex3 &operator-=(const Vertex3 &v);

    inline Vertex3 &operator*=(real s);

    inline Vertex3 &operator/=(real s);

    // Arithmetic methods
    inline Vertex3 operator+(const Vertex3 &v) const;

    inline Vertex3 operator-(const Vertex3 &v) const;

    inline Vertex3 operator-() const;

    inline Vertex3 operator*(real s) const;

    inline Vertex3 operator/(real s) const;

    inline real operator*(const Vertex3 &v) const;

    inline Vertex3 operator^(const Vertex3 &v) const;


#ifdef _GLIBCXX_IOSTREAM
    // Input/Output methods
    friend std::ostream& operator<<(std::ostream&, const Vertex3&);
    friend std::istream& operator>>(std::istream&, Vertex3&);
#endif

    // Additional vector methods
    inline real length();

    inline real norm();

    inline real norm2();

    inline real unitize();
};


inline void Vertex3::copy(const Vertex3 &v) {
    elt[0] = v.elt[0];
    elt[1] = v.elt[1];
    elt[2] = v.elt[2];
}

inline Vertex3 &Vertex3::operator=(const Vertex3 &v) {
    copy(v);
    return *this;
}

inline Vertex3 &Vertex3::operator+=(const Vertex3 &v) {
    elt[0] += v[0];
    elt[1] += v[1];
    elt[2] += v[2];
    return *this;
}

inline Vertex3 &Vertex3::operator-=(const Vertex3 &v) {
    elt[0] -= v[0];
    elt[1] -= v[1];
    elt[2] -= v[2];
    return *this;
}

inline Vertex3 &Vertex3::operator*=(real s) {
    elt[0] *= s;
    elt[1] *= s;
    elt[2] *= s;
    return *this;
}

inline Vertex3 &Vertex3::operator/=(real s) {
    elt[0] /= s;
    elt[1] /= s;
    elt[2] /= s;
    return *this;
}

///////////////////////

inline Vertex3 Vertex3::operator+(const Vertex3 &v) const {
    Vertex3 w(elt[0] + v[0], elt[1] + v[1], elt[2] + v[2]);
    return w;
}

inline Vertex3 Vertex3::operator-(const Vertex3 &v) const {
    Vertex3 w(elt[0] - v[0], elt[1] - v[1], elt[2] - v[2]);
    return w;
}

inline Vertex3 Vertex3::operator-() const {
    return Vertex3(-elt[0], -elt[1], -elt[2]);
}

inline Vertex3 Vertex3::operator*(real s) const {
    Vertex3 w(elt[0] * s, elt[1] * s, elt[2] * s);
    return w;
}

inline Vertex3 Vertex3::operator/(real s) const {
    Vertex3 w(elt[0] / s, elt[1] / s, elt[2] / s);
    return w;
}

inline real Vertex3::operator*(const Vertex3 &v) const {
    return elt[0] * v[0] + elt[1] * v[1] + elt[2] * v[2];
}

inline Vertex3 Vertex3::operator^(const Vertex3 &v) const {
    Vertex3 w(elt[1] * v[2] - v[1] * elt[2],
              -elt[0] * v[2] + v[0] * elt[2],
              elt[0] * v[1] - v[0] * elt[1]);
    return w;
}

inline real Vertex3::length() {
    return norm();
}

inline real Vertex3::norm() {
    return sqrt(elt[0] * elt[0] + elt[1] * elt[1] + elt[2] * elt[2]);
}

inline real Vertex3::norm2() {
    return elt[0] * elt[0] + elt[1] * elt[1] + elt[2] * elt[2];
}

inline real Vertex3::unitize() {
    real l = norm();
    if (l != 1.0)
        (*this) /= l;
    return l;
}

#ifdef _GLIBCXX_IOSTREAM
inline std::ostream& operator<<(std::ostream& out, const Vertex3& v)
{
    return out << "[" << v[0] << " " << v[1] << " " << v[2] << "]";
}

inline std::istream& operator>>(std::istream& in, Vertex3& v)
{
    return in >> (char *)"[" >> v[0] >> v[1] >> v[2] >> (char *)"]";
}
#endif


#endif
