#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stddef.h>

#include "Scene/CoordsSystem.hpp"

namespace Scene
{

struct Vector
{
    double dx, dy, dz;

    Vector(double dx, double dy, double dz) : dx(dx), dy(dy), dz(dz) {}
    Vector(const Point& begin, const Point& end) : 
        dx(end.x - begin.x), dy(end.y - begin.y), dz(end.z - begin.z) {}

    double length() const;
    void   length(const double newLength);
    
    Vector reflectRelatively(const Vector& pivot) const;
    Vector getNormalizedVector() const;

    operator Point() const { return Point(dx, dy, dz); }
};

Vector operator -(const Vector& self);
Vector operator +(const Vector& self, const Vector& other);
Vector operator -(const Vector& self, const Vector& other);
Vector operator *(const Vector& self, const double coeff);
Vector operator *(const double coeff, const Vector& self);
Vector operator /(const Vector& self, const double coeff);
double operator ^(const Vector& self, const Vector& other);

double cos(const Vector& v1, const Vector& v2);

} // Scene

#endif // VECTOR_HPP