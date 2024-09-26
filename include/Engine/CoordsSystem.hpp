#ifndef ENGINE_COORDS_SYSTEM_HPP
#define ENGINE_COORDS_SYSTEM_HPP

#include "Graphics/GraphicsWindow.hpp"

namespace Engine
{

struct Point
{
    double x, y, z;

    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    //Point(Graphics::WindowPoint windowPoint) : x(windowPoint.x), y(windowPoint.y), z(0) {}

    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);
};

Point operator+(const Point& self, const Point& other);
Point operator-(const Point& self, const Point& other);

class CoordsSystem
{
    Point center_;
    unsigned int stepInPixels_;

public:
    CoordsSystem(const unsigned int stepInPixels, const Point& centerInPixels) : 
        stepInPixels_(stepInPixels), center_(centerInPixels) {}

    void moveCenter (const Vector& deltaPixels);
    void changeScale(const int delta);

    unsigned int getSizeInPixels(const unsigned int size) const;
    Graphics::WindowVector getScaleInPixels(const Vector& scale) const;

    Graphics::WindowPoint getPosInWindow(const Point& point) const;

    Point getPosInCoordsSystem(const Graphics::WindowPoint& point) const;
};

double getDistanceSquare3D(const Point& p1, const Point& p2);
double getDistanceSquare2D(const Point& p1, const Point& p2);
double getDistance3D      (const Point& p1, const Point& p2);
double getDistance2D      (const Point& p1, const Point& p2);

} // namespace Engine

#endif // ENGINE_COORDS_SYSTEM_HPP