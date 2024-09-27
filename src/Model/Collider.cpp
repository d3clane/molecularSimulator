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

    double c1X = collider1.topLeft_->x;
    double c2X = collider2.topLeft_->x;

    double c1Y = collider1.topLeft_->y;
    double c2Y = collider2.topLeft_->y;

    double c1W = *collider1.width_;
    double c2W = *collider2.width_;

    double c1H = *collider1.height_;
    double c2H = *collider2.height_;

    return c2X < c1X + c1W && c1X < c2X + c2W && c2Y < c1Y + c1H && c1Y < c2Y + c2H;
}


} // namespace Model