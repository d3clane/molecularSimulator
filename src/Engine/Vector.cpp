#include <cassert>
#include <cmath>

#include "Engine/Vector.hpp"

namespace Engine
{

double cos(const Vector& v1, const Vector& v2)
{
    assert(v1.length() != 0);
    assert(v2.length() != 0);
    
    return v1 ^ v2 / v1.length() / v2.length(); 
}

double Vector::length() const
{
    assert(std::isfinite(dx));
    assert(std::isfinite(dy));
    assert(std::isfinite(dz));
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
    assert(length() != 0);
    assert(pivot.length() != 0);
    assert(std::isfinite(length()));
    assert(std::isfinite(pivot.length()));

    return 2 * projectOnto(pivot) - *this;
}

Vector Vector::getPerpendicular() const
{
    return Vector(dy, -dx, 0);
}

Vector Vector::projectOnto(const Vector& other) const
{
    double myLen = length();
    if (myLen == 0) return *this; // TODO: double comparison
    assert(myLen != 0);
    assert(other.length() != 0);

    return other.getNormalizedVector() * cos(*this, other) * length();
}

Vector& Vector::operator+=(const Vector& other)
{
    dx += other.dx;
    dy += other.dy;
    dz += other.dz;
    return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
    dx -= other.dx;
    dy -= other.dy;
    dz -= other.dz;
    return *this;
}

Vector& Vector::operator*=(const double coeff)
{
    assert(std::isfinite(coeff));

    dx *= coeff;
    dy *= coeff;
    dz *= coeff;
    return *this;
}

Vector& Vector::operator/=(const double coeff)
{
    assert(std::isfinite(coeff));
    assert(coeff != 0);

    dx /= coeff;
    dy /= coeff;
    dz /= coeff;
    return *this;
}

Vector operator -(const Vector& self)
{
    return Vector(-self.dx, -self.dy, -self.dz);
}

Vector operator +(const Vector& self, const Vector& other)
{
    Vector tmp = self;
    return tmp += other;
}

Vector operator -(const Vector& self, const Vector& other)
{
    Vector tmp = self;
    return tmp -= other;
}

Vector operator *(const Vector& self, const double coeff)
{
    Vector tmp = self;
    return tmp *= coeff;
}

Vector operator *(const double coeff, const Vector& self)
{
    return self * coeff;
}

Vector operator /(const Vector& self, const double coeff)
{
    Vector tmp = self;
    return tmp /= coeff;
}

double operator ^(const Vector& self, const Vector& other)
{
    return self.dx * other.dx + self.dy * other.dy + self.dz * other.dz;
}

} // namespace Engine
