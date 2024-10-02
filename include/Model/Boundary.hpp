#ifndef MODEL_BOUNDARY_HPP
#define MODEL_BOUNDARY_HPP

#include "Engine/Transformable.hpp"
#include "Engine/Vector.hpp"
#include "Model/Collider.hpp"

namespace Simulator
{

class Boundary : public Engine::Transformable
{
    double width_;
    double height_;

    std::unique_ptr<RectangleCollider> collider_;

    Engine::Vector perpendicular_;

public:
    Boundary(
        const Point& topLeft, const double width, const double height,
        const Engine::Vector& perpendicular
    );

    Boundary(const Boundary& other);
    
    const RectangleCollider* collider() const &;
    const Engine::Vector& perpendicular() const &;
};

} // namespace Simulator

#endif // MODEL_BOUNDARY_HPP