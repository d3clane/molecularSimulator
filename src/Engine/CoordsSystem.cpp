#include "Graphics/PixelsArray.hpp"
#include "Engine/CoordsSystem.hpp"
#include "Engine/Vector.hpp"

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

void CoordsSystem::moveCenter(const Vector& delta)
{
    center_ += delta;
}

void CoordsSystem::changeScale(const int delta)
{
    changeOneArgScale(stepInPixels_, delta);
    changeOneArgScale(stepInPixels_, delta);
}

unsigned int CoordsSystem::getSizeInPixels(const double size) const
{
    return size * stepInPixels_;
}

Graphics::WindowVector CoordsSystem::getScaleInPixels(const Vector& scale) const
{
    return Graphics::WindowVector{getSizeInPixels(scale.dx), getSizeInPixels(scale.dy)};
}

Graphics::WindowPoint CoordsSystem::getPosInWindow(const Point& point) const
{
    //std::cout << "ME CENTER - " << center_.x << " " << center_.y << " POINT Y" << point.y << " AFTER SUM: " << center_.x + point.x * stepInPixels_ << " " << center_.y + point.y * stepInPixels_ << std::endl;
    return Graphics::WindowPoint(center_.x + point.x * stepInPixels_, center_.y + point.y * stepInPixels_);
}

Point CoordsSystem::getPosInCoordsSystem(const Graphics::WindowPoint& point) const
{
    return Point((point.x - center_.x) / stepInPixels_, 
                 (point.y - center_.y) / stepInPixels_, 
                 0);
}

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