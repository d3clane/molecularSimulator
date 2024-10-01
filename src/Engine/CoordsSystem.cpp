#include "Graphics/PixelsArray.hpp"
#include "Engine/CoordsSystem.hpp"
#include "Engine/Vector.hpp"
#include "Utils/Doubles.hpp"

#include <iostream>

namespace Engine
{

namespace 
{

void changeOneArgScale(unsigned int& step, const int delta);

void changeOneArgScale(unsigned int& step, const int delta)
{
    if (delta < 0 && step <= -delta) step = 1;
    else step += delta;
}

} // anonymous namespace

using Graphics::WindowLine;

Point& Point::operator+=(const Point& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

Point& Point::operator-=(const Point& other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

Point operator+(const Point& self, const Point& other)
{
    Point selfCpy = self;
    return selfCpy += other;
}

Point operator-(const Point& self, const Point& other)
{
    Point selfCpy = self;
    return selfCpy -= other;
}

CoordsSystem::CoordsSystem(
    const Vector& xBasisVector, const Vector& yBasisVector, const Vector& zBasisVector,
    const Point& center
) : center_(center), 
    xBasisVector_(xBasisVector), 
    yBasisVector_(yBasisVector), 
    zBasisVector_(zBasisVector)
{
    // TODO: assert on linear independence
    assert(
        Utils::compare(xBasisVector.lengthSquare(), yBasisVector.lengthSquare()) == 
            Utils::ComparisonResult::Equal &&
        Utils::compare(xBasisVector.lengthSquare(), zBasisVector.lengthSquare()) ==
            Utils::ComparisonResult::Equal
    );
    
    // TODO: IS_OK for classes like point, vector etc
}

CoordsSystem::CoordsSystem(const unsigned int stepInPixels, const Point& center) :
    CoordsSystem({stepInPixels, 0, 0}, {0, stepInPixels, 0}, {0, 0, stepInPixels}, center)
{
}

void CoordsSystem::moveCenter(const Vector& delta)
{
    center_ += delta;
}

#if 0
void CoordsSystem::changeScale(const int delta)
{
    changeOneArgScale(stepInPixels_, delta);
    changeOneArgScale(stepInPixels_, delta);
}
#endif 

double CoordsSystem::getSizeInPixels(const double size) const
{
    assert(std::isfinite(size));
    assert(size >= 0);
    assert(xBasisVector_.length() == yBasisVector_.length() && 
           xBasisVector_.length() == zBasisVector_.length());
    
    return getVectorInBaseCoordsSystem(Vector{size, 0, 0}).length();
}

Graphics::WindowVector CoordsSystem::getScaleInPixels(const Vector& scale) const
{
    Vector scaleInPixels = getVectorInBaseCoordsSystem(scale);
    return Graphics::WindowVector{scaleInPixels.dx, scaleInPixels.dy};
}

Vector CoordsSystem::getVectorInBaseCoordsSystem(const Vector& vector) const
{
    return xBasisVector_ * vector.dx + yBasisVector_ * vector.dy + zBasisVector_ * vector.dz;
}

Point CoordsSystem::getPointInBaseCoordsSystem(const Point& point) const
{
    return center_ + getVectorInBaseCoordsSystem(Vector{point});
}

Graphics::WindowPoint CoordsSystem::getPointInWindow(const Point& point) const
{
    Point baseCsPoint = getPointInBaseCoordsSystem(point);

    return Graphics::WindowPoint{baseCsPoint.x, baseCsPoint.y};
}

#if 0
Point CoordsSystem::getPointInCoordsSystem(const Graphics::WindowPoint& point) const
{
    return Point((point.x - center_.x) / stepInPixels_, 
                 (point.y - center_.y) / stepInPixels_, 
                 0);
}
#endif

double getDistance3D(const Point& p1, const Point& p2)
{
    return sqrt(getDistance3D(p1, p2));
}

double getDistance2D(const Point& p1, const Point& p2)
{
    return sqrt(getDistanceSquare2D(p1, p2));
}

double getDistanceSquare2D(const Point& p1, const Point& p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + 
           (p1.y - p2.y) * (p1.y - p2.y);
}

double getDistanceSquare3D(const Point& p1, const Point& p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + 
           (p1.y - p2.y) * (p1.y - p2.y) + 
           (p1.z - p2.z) * (p1.z - p2.z);
}

} // namespace Engine