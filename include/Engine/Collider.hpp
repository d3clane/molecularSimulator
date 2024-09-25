#ifndef ENGINE_COLLIDER_HPP
#define ENGINE_COLLIDER_HPP

#include "Engine/CoordsSystem.hpp"

#include <vector>

namespace Engine
{

class RectangleCollider;

class CircleCollider
{
    Point center_;
    double radius_;

public:
    CircleCollider(const Point& center, const double radius);

    friend bool checkCollision(const CircleCollider& collider1, const CircleCollider& collider2);
    friend bool checkCollision(const CircleCollider& collider,  const RectangleCollider& collider2);
};

class RectangleCollider
{
    Point topLeft_;
    double width_, height_;

public:
    RectangleCollider(const Point& topLeft, const double width, const double height);

    friend bool checkCollision(const RectangleCollider& collider1, const RectangleCollider& collider2);
    friend bool checkCollision(const CircleCollider& collider,     const RectangleCollider& collider2);
};

bool checkCollision(const RectangleCollider& collider1, const RectangleCollider& collider2);
bool checkCollision(const CircleCollider& collider1,    const CircleCollider& collider2   );
bool checkCollision(const CircleCollider& collider,     const RectangleCollider& collider2);
bool checkCollision(const RectangleCollider& collider,  const CircleCollider& collider2   );

} // namespace Engine

#endif // ENGINE_COLLIDER_HPP