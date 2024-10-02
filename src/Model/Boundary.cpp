#include "Model/Boundary.hpp"

namespace Simulator
{

Boundary::Boundary(
    const Point& topLeft, const double width, const double height,
    const Engine::Vector& perpendicular
) : Engine::Transformable(topLeft), width_(width), height_(height), perpendicular_(perpendicular),
    collider_(new RectangleCollider(&transformablePos_, &width_, &height_))
{
}

Boundary::Boundary(const Boundary& other) : 
    Boundary(other.transformablePos_, other.width_, other.height_, other.perpendicular_)
{
}

const RectangleCollider* Boundary::collider() const &   { return collider_.get(); }
const Engine::Vector& Boundary::perpendicular() const & { return perpendicular_;  }

} // namespace Simulator