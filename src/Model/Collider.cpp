#include "Engine/CoordsSystem.hpp"
#include "Model/Collider.hpp"

#include <iostream>
#include <cstdio>

namespace Model
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

CircleCollider::CircleCollider(const Point* topLeft, const double* radius) : topLeft_(topLeft), radius_(radius) 
{
}

RectangleCollider::RectangleCollider(const Point* topLeft, const double* width, const double* height) :
    topLeft_(topLeft), width_(width), height_(height)
{
}

bool checkCollisionCircleCircle(const Collider* circleCollider1, const Collider* circleCollider2)
{
    const CircleCollider& collider1 = *dynamic_cast<const CircleCollider*>(circleCollider1);
    const CircleCollider& collider2 = *dynamic_cast<const CircleCollider*>(circleCollider2);

    double criticalDistance = *collider1.radius_ + *collider2.radius_;

    return getDistanceSquare2D(*collider1.topLeft_, *collider2.topLeft_) < criticalDistance * criticalDistance;
}

bool checkCollisionCircleRect(const Collider* circleCollider1,  const Collider* rectangleCollider2)
{
    const CircleCollider& collider1    = *dynamic_cast<const CircleCollider*>(circleCollider1);
    const RectangleCollider& collider2 = *dynamic_cast<const RectangleCollider*>(rectangleCollider2);

    bool condition = 
        checkCollisionCircleToRectangleOneSide(
            collider2.topLeft_->x, *collider1.radius_, 
            collider2.topLeft_->y, collider2.topLeft_->y + *collider2.height_, 
            collider1.topLeft_->y + *collider1.radius_, collider1.topLeft_->x + *collider1.radius_) |
        checkCollisionCircleToRectangleOneSide(
            collider2.topLeft_->x + *collider2.width_, *collider1.radius_, 
            collider2.topLeft_->y, collider2.topLeft_->y + *collider2.height_, 
            collider1.topLeft_->y + *collider1.radius_, collider1.topLeft_->x + *collider1.radius_) |
        checkCollisionCircleToRectangleOneSide(
            collider2.topLeft_->y, *collider1.radius_, 
            collider2.topLeft_->x, collider2.topLeft_->x + *collider2.width_, 
            collider1.topLeft_->x + *collider1.radius_, collider1.topLeft_->y + *collider1.radius_) |
        checkCollisionCircleToRectangleOneSide(
            collider2.topLeft_->y + *collider2.height_, *collider1.radius_, 
            collider2.topLeft_->x, collider2.topLeft_->x + *collider2.width_, 
            collider1.topLeft_->x + *collider1.radius_, collider1.topLeft_->y + *collider1.radius_);


    return condition;
}

bool checkCollisionRectCircle(const Collider* rectangleCollider1, const Collider* circleCollider2)
{
    return checkCollisionCircleRect(circleCollider2, rectangleCollider1);
}

bool checkCollisionRectRect(const Collider* rectangleCollider1, const Collider* rectangleCollider2)
{
    const RectangleCollider& collider1 = *dynamic_cast<const RectangleCollider*>(rectangleCollider1);
    const RectangleCollider& collider2 = *dynamic_cast<const RectangleCollider*>(rectangleCollider2);
    
    return std::abs(collider1.topLeft_->x - collider2.topLeft_->x) < 
                        (*collider1.width_  + *collider2.width_ ) / 2 &
           std::abs(collider1.topLeft_->y - collider2.topLeft_->y) < 
                        (*collider1.height_ + *collider2.height_) / 2;
}


} // namespace Model