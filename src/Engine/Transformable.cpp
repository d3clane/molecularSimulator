#include "Engine/Transformable.hpp"
#include "Engine/Vector.hpp"

namespace Engine
{

Transformable::Transformable(const Point& topLeft) : transformableTopLeft_(topLeft)
{
}

void Transformable::move(const Vector& delta)
{
    transformableTopLeft_ += delta;
}

void Transformable::pos(const Point& pos)
{
    transformableTopLeft_ = pos;
}

Point Transformable::pos () const
{
    return transformableTopLeft_;
}

} // namespace Engine