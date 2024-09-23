#include <cassert>
#include <cmath>

#include "Scene/Vector.hpp"

namespace Scene
{

double cos(const Vector& v1, const Vector& v2)
{
    return v1 ^ v2 / v1.length() / v2.length(); 
}

double Vector::length() const
{
    return sqrt(dx * dx + dy * dy + dz * dz);
}

void Vector::length(const double newLength)
{
    double oldLength = length();
    assert(oldLength != 0);

    double coeff = newLength / oldLength;

    dx *= coeff;
    dy *= coeff;
    dz *= coeff;
}

Vector Vector::getNormalizedVector() const
{
    double len = length();

    if (len == 0)
        return Vector(0, 0, 0);

    return Vector(dx / len, dy / len, dz / len);
}

Vector Vector::reflectRelatively(const Vector& pivot) const
{
    Vector normalizedPivot = pivot.getNormalizedVector();

    double pivotLength = pivot.length();
    double cosAngle = cos(*this, normalizedPivot);

    return normalizedPivot * (2 * pivotLength * cosAngle) - *this;
}

Vector operator -(const Vector& self)
{
    return Vector(-self.dx, -self.dy, -self.dz);
}

Vector operator +(const Vector& self, const Vector& other)
{
    return Vector(self.dx + other.dx, self.dy + other.dy, self.dz + other.dz);
}

Vector operator -(const Vector& self, const Vector& other)
{
    return Vector(self.dx - other.dx, self.dy - other.dy, self.dz - other.dz);
}

Vector operator *(const Vector& self, const double coeff)
{
    assert(std::isfinite(coeff));

    return Vector(self.dx * coeff, self.dy * coeff, self.dz * coeff);
}

Vector operator *(const double coeff, const Vector& self)
{
    return self * coeff;
}

Vector operator /(const Vector& self, const double coeff)
{
    assert(std::isfinite(coeff));
    assert(coeff != 0);

    return Vector(self.dx / coeff, self.dy / coeff, self.dz / coeff);
}

double operator ^(const Vector& self, const Vector& other)
{
    return self.dx * other.dx + self.dy * other.dy + self.dz * other.dz;
}

} // Scene