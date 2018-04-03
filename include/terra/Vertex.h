#ifndef TERRA_VERTEX_H // -*- C++ -*-
#define TERRA_VERTEX_H

class Vertex {
protected:
    double elt[2]{};

    inline void copy(const Vertex &v);

public:
    // Standard constructors
    explicit Vertex(double x = 0, double y = 0) {
        elt[0] = x;
        elt[1] = y;
    }

    Vertex(const Vertex &v) { copy(v); }

    explicit Vertex(const double *v) {
        elt[0] = v[0];
        elt[1] = v[1];
    }

    Vertex &clone() const { return *(new Vertex(elt[0], elt[1])); }

    // Access methods
    double &operator()(int i) { return elt[i]; }

    const double &operator()(int i) const { return elt[i]; }

    double &operator[](int i) { return elt[i]; }

    const double &operator[](int i) const { return elt[i]; }

    // Assignment methods
    inline Vertex &operator=(const Vertex &v);

    inline Vertex &operator+=(const Vertex &v);

    inline Vertex &operator-=(const Vertex &v);

    inline Vertex &operator*=(double s);

    inline Vertex &operator/=(double s);

    // Arithmetic methods
    inline Vertex operator+(const Vertex &v) const;

    inline Vertex operator-(const Vertex &v) const;

    inline Vertex operator-() const;

    inline Vertex operator*(double s) const;

    inline Vertex operator/(double s) const;

    inline double operator*(const Vertex &v) const;

#ifdef _GLIBCXX_IOSTREAM
    // Input/Output methods
    friend std::ostream& operator<<(std::ostream&, const Vertex&);
    friend std::istream& operator>>(std::istream&, Vertex&);
#endif

    // Additional vector methods
    inline double length();

    inline double norm();

    inline double norm2();

    inline int operator==(const Vertex &v) const {
        return (*this - v).norm2() < EPS2;
    }
};


inline void Vertex::copy(const Vertex &v) {
    elt[0] = v.elt[0];
    elt[1] = v.elt[1];
}

inline Vertex &Vertex::operator=(const Vertex &v) {
    copy(v);
    return *this;
}

inline Vertex &Vertex::operator+=(const Vertex &v) {
    elt[0] += v[0];
    elt[1] += v[1];
    return *this;
}

inline Vertex &Vertex::operator-=(const Vertex &v) {
    elt[0] -= v[0];
    elt[1] -= v[1];
    return *this;
}

inline Vertex &Vertex::operator*=(double s) {
    elt[0] *= s;
    elt[1] *= s;
    return *this;
}

inline Vertex &Vertex::operator/=(double s) {
    elt[0] /= s;
    elt[1] /= s;
    return *this;
}

///////////////////////

inline Vertex Vertex::operator+(const Vertex &v) const {
    Vertex w(elt[0] + v[0], elt[1] + v[1]);
    return w;
}

inline Vertex Vertex::operator-(const Vertex &v) const {
    Vertex w(elt[0] - v[0], elt[1] - v[1]);
    return w;
}

inline Vertex Vertex::operator-() const {
    return Vertex(-elt[0], -elt[1]);
}

inline Vertex Vertex::operator*(double s) const {
    Vertex w(elt[0] * s, elt[1] * s);
    return w;
}

inline Vertex Vertex::operator/(double s) const {
    Vertex w(elt[0] / s, elt[1] / s);
    return w;
}

inline double Vertex::operator*(const Vertex &v) const {
    return elt[0] * v[0] + elt[1] * v[1];
}

inline double Vertex::length() {
    return norm();
}

inline double Vertex::norm() {
    return sqrt(elt[0] * elt[0] + elt[1] * elt[1]);
}

inline double Vertex::norm2() {
    return elt[0] * elt[0] + elt[1] * elt[1];
}

#ifdef _GLIBCXX_IOSTREAM
inline std::ostream& operator<<(std::ostream& out, const Vertex& v)
{
    return out << (char *)"[" << v[0] << (char *)" " << v[1] << (char *)"]";
}

inline std::istream& operator>>(std::istream& in, Vertex& v)
{
    return in >> (char *)"[" >> v[0] >> v[1] >> (char *)"]";
}
#endif


#endif
