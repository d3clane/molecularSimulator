#ifndef ENGINE_COLLIDER_HPP
#define ENGINE_COLLIDER_HPP

#include "Engine/CoordsSystem.hpp"

#include <vector>

namespace Engine
{



class RectangleCollider;

class CircleCollider
{
    const Point* topLeft_;
    const double* radius_;

public:
    CircleCollider(const Point* topLeft, const double* radius);

    CircleCollider(const CircleCollider& other) = delete;

    friend bool checkCollision(const CircleCollider& collider1, const CircleCollider& collider2);
    friend bool checkCollision(const CircleCollider& collider,  const RectangleCollider& collider2);
};

class RectangleCollider
{
    const Point* topLeft_;
    const double* width_, *height_;

public:
    RectangleCollider(const Point* topLeft, const double* width, const double* height);
    
    RectangleCollider(const RectangleCollider& other) = delete;

    friend bool checkCollision(const RectangleCollider& collider1, const RectangleCollider& collider2);
    friend bool checkCollision(const CircleCollider& collider,     const RectangleCollider& collider2);
};

bool checkCollision(const RectangleCollider& collider1, const RectangleCollider& collider2);
bool checkCollision(const CircleCollider& collider1,    const CircleCollider& collider2   );
bool checkCollision(const CircleCollider& collider,     const RectangleCollider& collider2);
bool checkCollision(const RectangleCollider& collider,  const CircleCollider& collider2   );

} // namespace Engine

#endif // ENGINE_COLLIDER_HPP