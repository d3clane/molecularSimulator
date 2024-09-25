#include "Scene/Molecule.hpp"

#include <iostream>
#include <cstdio>

namespace Scene
{

Molecule::Molecule(
    const Graphics::Sprite& sprite, const Point& topLeft, const unsigned int mass, const Vector& speed
) : sprite_(sprite), mass_(mass), speed_(speed), Engine::Transformable(topLeft)
{
    sprite_.setPosition({transformableTopLeft_.x, transformableTopLeft_.y});
    //std::cout << transformableTopLeft_.x << " " << transformableTopLeft_.y << '\n';
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
    collider_(Engine::CircleCollider(&transformableTopLeft_, &radius_))
{
    //printf("this addr - %p, topLeft addr - %p\n", this, &transformableTopLeft_);

    sprite_.scaleInPixels({radius * 2, radius * 2});
}

CircleMolecule::CircleMolecule(const CircleMolecule& other) : 
    CircleMolecule(other.radius_, Molecule::CtorParams(
            other.sprite_, other.transformableTopLeft_, other.mass_, other.speed_
        )
    )
{
    sprite_ = other.sprite_; // TODO: think why it doesn't work with the init list already
    sprite_.setPosition({transformableTopLeft_.x, transformableTopLeft_.y});
}

CircleMolecule::CircleMolecule(CircleMolecule&& other) : CircleMolecule(other)
{
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
    collider_(Engine::RectangleCollider(&transformableTopLeft_, &width_, &height_))
{
    sprite_.scaleInPixels({width, height});
}

Engine::RectangleCollider& RectangleMolecule::collider() & { return collider_; }

} // namespace Scene