#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

namespace Scene
{

class Vector;
class Point;

class Transformable
{
public:
    virtual void move  (const Vector& delta) = 0;
    virtual void setPos(const Point&  pos  ) = 0;
};

} // namespace Scene

#endif // TRANSFORMABLE_HPP