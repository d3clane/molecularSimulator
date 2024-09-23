#include "Graphics/PixelsArray.hpp"
#include "Scene/CoordsSystem.hpp"

namespace Scene
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

Point operator +(const Point& self, const Point& other)
{
    return Point(self.x + other.x, self.y + other.y, self.z + other.z);
}

void CoordsSystem::moveCenter(const PixelVector& delta)
{
    center_.x += delta.dx;
    center_.y += delta.dy;
    center_.z += delta.dz;
}

void CoordsSystem::changeScale(const int delta)
{
    changeOneArgScale(stepInPixels_, delta);
    changeOneArgScale(stepInPixels_, delta);
}

Graphics::WindowPoint CoordsSystem::getPosInWindow(const Point& point) const
{
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
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + 
                (p1.y - p2.y) * (p1.y - p2.y) + 
                (p1.z - p2.z) * (p1.z - p2.z));
}

double getDistance2D(const Point& p1, const Point& p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + 
                (p1.y - p2.y) * (p1.y - p2.y));
}

} // Scene