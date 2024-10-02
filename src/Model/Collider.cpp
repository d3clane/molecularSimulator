#include "Engine/CoordsSystem.hpp"
#include "Model/Collider.hpp"

#include <iostream>
#include <cstdio>

namespace Simulator
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

bool circleIsInsideRectangle(
    const Engine::Point& topLeftCircle, 
    const Engine::Point& topLeftCollider, const Engine::Point& bottomRightCollider
)
{
    return topLeftCollider.x <= topLeftCircle.x && topLeftCircle.x <= bottomRightCollider.x &&
           topLeftCollider.y <= topLeftCircle.y && topLeftCircle.y <= bottomRightCollider.y;
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
    assert(circleCollider1);
    assert(circleCollider2);

    const CircleCollider& collider1 = *static_cast<const CircleCollider*>(circleCollider1);
    const CircleCollider& collider2 = *static_cast<const CircleCollider*>(circleCollider2);

    double criticalDistance = *collider1.radius_ + *collider2.radius_;

    return getDistanceSquare2D(*collider1.topLeft_, *collider2.topLeft_) < criticalDistance * criticalDistance;
}

bool checkCollisionCircleRect(const Collider* circleCollider,  const Collider* rectangleCollider)
{
    assert(circleCollider);
    assert(rectangleCollider);

    const CircleCollider& collider1    = *static_cast<const CircleCollider*>(circleCollider);
    const RectangleCollider& collider2 = *static_cast<const RectangleCollider*>(rectangleCollider);

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
            collider1.topLeft_->x + *collider1.radius_, collider1.topLeft_->y + *collider1.radius_) |
        circleIsInsideRectangle(
            *collider1.topLeft_, *collider2.topLeft_, 
            *collider2.topLeft_ + Engine::Point{*collider2.width_, *collider2.height_, 0}
        );


    return condition;
}

bool checkCollisionRectCircle(const Collider* rectangleCollider, const Collider* circleCollider)
{
    return checkCollisionCircleRect(circleCollider, rectangleCollider);
}

bool checkCollisionRectRect(const Collider* rectangleCollider1, const Collider* rectangleCollider2)
{
    assert(rectangleCollider1);
    assert(rectangleCollider2);

    const RectangleCollider& collider1 = *static_cast<const RectangleCollider*>(rectangleCollider1);
    const RectangleCollider& collider2 = *static_cast<const RectangleCollider*>(rectangleCollider2);

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

} // namespace Simulator