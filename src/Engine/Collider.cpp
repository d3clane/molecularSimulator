#include "Engine/Collider.hpp"
#include "Engine/CoordsSystem.hpp"

#include <iostream>

namespace Engine
{

namespace
{

bool checkCollisionCircleToRectangleOneSide(
    const double perpendicularDirectionCoordOnRectangle, const double radius,
    const double minParallelDirectionCoordOnRectangle, const double maxParallelDirectionCoordOnRectangle,
    const double parallelDirectionCircleCenterCoord, const double perpendicularDirectionCircleCenterCoord
)
{

    bool condition = ((minParallelDirectionCoordOnRectangle - radius < parallelDirectionCircleCenterCoord) &
                     (parallelDirectionCircleCenterCoord < maxParallelDirectionCoordOnRectangle + radius));

    condition &= ((perpendicularDirectionCircleCenterCoord >= perpendicularDirectionCoordOnRectangle - radius) &
                 (perpendicularDirectionCircleCenterCoord <= perpendicularDirectionCoordOnRectangle + radius));

    return condition;
}

} // namespace anonymous

CircleCollider::CircleCollider(const Point& center, const double radius) : center_(center), radius_(radius) 
{
}

RectangleCollider::RectangleCollider(const Point& topLeft, const double width, const double height) :
    topLeft_(topLeft), width_(width), height_(height)
{
}


bool checkCollision(const CircleCollider& collider1, const CircleCollider& collider2)
{
    double criticalDistance = collider1.radius_ + collider2.radius_;
    return getDistanceSquare2D(collider1.center_, collider2.center_) < criticalDistance * criticalDistance;
}

bool checkCollision(const CircleCollider& collider1,  const RectangleCollider& collider2)
{
    bool condition = 
        checkCollisionCircleToRectangleOneSide(
            collider2.topLeft_.x, collider1.radius_, 
            collider2.topLeft_.y, collider2.topLeft_.y + collider2.height_, 
            collider1.center_.y, collider1.center_.x) |
        checkCollisionCircleToRectangleOneSide(
            collider2.topLeft_.x + collider2.width_, collider1.radius_, 
            collider2.topLeft_.y, collider2.topLeft_.y + collider2.height_, 
            collider1.center_.y, collider1.center_.x) |
        checkCollisionCircleToRectangleOneSide(
            collider2.topLeft_.y, collider1.radius_, 
            collider2.topLeft_.x, collider2.topLeft_.x + collider2.width_, 
            collider1.center_.x, collider1.center_.y) |
        checkCollisionCircleToRectangleOneSide(
            collider2.topLeft_.y + collider2.height_, collider1.radius_, 
            collider2.topLeft_.x, collider2.topLeft_.x + collider2.width_, 
            collider1.center_.x, collider1.center_.y);


    return condition;
}

bool checkCollision(const RectangleCollider& collider1, const CircleCollider& collider2)
{
    return checkCollision(collider2, collider1);
}

bool checkCollision(const RectangleCollider& collider1, const RectangleCollider& collider2)
{
    return std::abs(collider1.topLeft_.x - collider2.topLeft_.x) < (collider1.width_  + collider2.width_ ) / 2 &
           std::abs(collider1.topLeft_.y - collider2.topLeft_.y) < (collider1.height_ + collider2.height_) / 2;
}


} // namespace Engine