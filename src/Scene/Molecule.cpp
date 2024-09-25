#include "Scene/Molecule.hpp"

namespace Scene
{

Molecule::Molecule(
    const Graphics::Sprite& sprite, const Point& topLeft, const unsigned int mass, const Vector& speed
) : sprite_(sprite), mass_(mass), speed_(speed), Engine::Transformable(topLeft)
{
    sprite_.setPosition({transformableTopLeft_.x, transformableTopLeft_.y});
}

Molecule::Molecule(const Molecule::CtorParams& ctorParams) : 
    Molecule(ctorParams.sprite, ctorParams.topLeft, ctorParams.mass, ctorParams.speed)
{
}

unsigned int  Molecule::mass() const                     { return mass_;  }
void          Molecule::mass(const unsigned int mass)    { mass_ = mass;  }
const Vector& Molecule::speed() const &                  { return speed_; }
void          Molecule::speed(const Vector& speed)       { speed_ = speed; }

Point Molecule::topLeft() const { return transformableTopLeft_; };
Molecule::operator Graphics::Sprite() { return sprite_; } // TODO: configure scale

CircleMolecule::CircleMolecule(const double radius, const Molecule::CtorParams& ctorParams) : 
    Molecule(ctorParams), radius_(radius),
    collider_(Engine::CircleCollider(ctorParams.topLeft + Point(radius, radius, 0), radius))
{
    sprite_.scaleInPixels({radius * 2, radius * 2});
}

Engine::CircleCollider& CircleMolecule::collider() & { return collider_; }
double CircleMolecule::radius() const { return radius_; }

CircleMolecule::operator Graphics::Sprite()
{
    sprite_.setPosition({transformableTopLeft_.x, transformableTopLeft_.y});
    //sprite_.scaleInPixels({2 * radius_, 2 * radius_});

    return sprite_;
}

RectangleMolecule::RectangleMolecule(
    const double width, const double height, const Molecule::CtorParams& ctorParams
) : Molecule(ctorParams), width_(width), height_(height),
    collider_(Engine::RectangleCollider(ctorParams.topLeft, width, height))
{
    sprite_.scaleInPixels({width, height});
}

Engine::RectangleCollider& RectangleMolecule::collider() & { return collider_; }

} // namespace Scene