#ifndef MODEL_COLLIDER_HPP
#define MODEL_COLLIDER_HPP

#include "Engine/CoordsSystem.hpp"

#include <vector>

namespace Model
{

class Collider;

using Engine::Point;
using CheckCollisionFuncType = bool (*)(const Collider* collider1, const Collider* collider2);

class Collider
{
public:
    virtual ~Collider() = default;
};

class RectangleCollider;

class CircleCollider : public Collider
{
    const Point* topLeft_;

    const double* radius_;
public:
    explicit CircleCollider(const Point* topLeft = nullptr, const double* radius = nullptr);

    CircleCollider(const Collider* other) = delete;

    friend bool checkCollisionCircleCircle(const Collider* collider1, const Collider* collider2);
    friend bool checkCollisionCircleRect  (const Collider* collider1, const Collider* collider2);
};

class RectangleCollider : public Collider
{
    const Point* topLeft_;
    const double* width_, *height_;

public:
    explicit RectangleCollider(
        const Point* topLeft = nullptr, const double* width = nullptr, const double* height = nullptr
    );
    
    RectangleCollider(const Collider* other) = delete;

    friend bool checkCollisionRectRect  (const Collider* collider1, const Collider* collider2);
    friend bool checkCollisionCircleRect(const Collider* collider1, const Collider* collider2);
};

bool checkCollisionRectRect    (const Collider* collider1, const Collider* collider2);
bool checkCollisionCircleCircle(const Collider* collider1, const Collider* collider2);
bool checkCollisionCircleRect  (const Collider* collider1, const Collider* collider2);
bool checkCollisionRectCircle  (const Collider* collider1, const Collider* collider2);

} // namespace Model

#endif // MODEL_COLLIDER_HPP