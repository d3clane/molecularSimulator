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

struct PixelVector
{
    int dx, dy, dz;

    PixelVector(int dx, int dy, int dz) : dx(dx), dy(dy), dz(dz) {}
};

class CoordsSystem
{
    Point center_;
    unsigned int stepInPixels_;

public:
    CoordsSystem(const unsigned int stepInPixels, const Point& center) : 
        stepInPixels_(stepInPixels), center_(center) {}

    void moveCenter (const PixelVector& delta);
    void changeScale(const int delta);

    void draw(Graphics::RenderWindow& window, const Graphics::Drawable* drawable);

    Graphics::WindowPoint getPosInWindow(const Point& point) const;

    Point getPosInCoordsSystem(const Graphics::WindowPoint& point) const;
};

double getDistanceSquare3D(const Point& p1, const Point& p2);
double getDistanceSquare2D(const Point& p1, const Point& p2);
double getDistance3D      (const Point& p1, const Point& p2);
double getDistance2D      (const Point& p1, const Point& p2);

} // namespace Engine

#endif // ENGINE_COORDS_SYSTEM_HPP