#include "Model/Molecule.hpp"

#include <iostream>
#include <cstdio>

namespace Simulator
{

double Molecule::basicSpeedAbs_;
double Molecule::basicMass_;
double CircleMolecule::basicRadius_;
double RectangleMolecule::basicHeight_;
double RectangleMolecule::basicWidth_;

Molecule::Molecule(
    const Point& topLeft, const double mass, const Vector& speed
) : mass_(mass), speed_(speed), Engine::Transformable(topLeft), id_(MoleculeType::Circle), collider_(nullptr)
{
}

Molecule::Molecule(const Molecule::CtorParams& ctorParams) : 
    Molecule(ctorParams.topLeft, ctorParams.mass, ctorParams.speed)
{
}

Molecule::Molecule(const Molecule& other) : Molecule(other.transformablePos_, other.mass_, other.speed_)
{
}

double        Molecule::mass() const                   { return mass_;       }
void          Molecule::mass(const double initMass)    { mass_ = initMass;   }
const Vector& Molecule::speed() const &                { return speed_;      }
void          Molecule::speed(const Vector& initSpeed) { speed_ = initSpeed; }

MoleculeType Molecule::id() const { return id_; };

const Collider* Molecule::collider() const & { return collider_.get(); };

Point Molecule::topLeft() const { return transformablePos_; };

double Molecule::basicSpeedAbs() { return Molecule::basicSpeedAbs_; }
double Molecule::basicMass()     { return Molecule::basicMass_;     }

void Molecule::basicSpeedAbs(const double newSpeed) { Molecule::basicSpeedAbs_ = newSpeed; }
void Molecule::basicMass    (const double newMass)  { Molecule::basicMass_     = newMass;  }

CircleMolecule::CircleMolecule(const double radius, const Molecule::CtorParams& ctorParams) : 
    Molecule(ctorParams), radius_(radius)
{
    id_ = MoleculeType::Circle;
    collider_.reset(new CircleCollider{&transformablePos_, &radius_});
}

CircleMolecule::CircleMolecule(const CircleMolecule& other) : 
    CircleMolecule(other.radius_, Molecule::CtorParams(other.transformablePos_, other.mass_, other.speed_))
{
}

CircleMolecule::CircleMolecule(CircleMolecule&& other) : CircleMolecule(other)
{
}

double CircleMolecule::radius() const           { return radius_;      }
void   CircleMolecule::radius(double newRadius) { radius_ = newRadius; }

double CircleMolecule::basicRadius() { return CircleMolecule::basicRadius_; }

void CircleMolecule::basicRadius(const double newRadius) { CircleMolecule::basicRadius_ = newRadius; }

RectangleMolecule::RectangleMolecule(
    const double width, const double height, const Molecule::CtorParams& ctorParams
) : Molecule{ctorParams}, width_(width), height_(height)
{
    id_ = MoleculeType::Rectangle;
    collider_.reset(new RectangleCollider(&transformablePos_, &width_, &height_));
}

RectangleMolecule::RectangleMolecule(const RectangleMolecule& other) : 
    RectangleMolecule(
        other.width_, other.height_, Molecule::CtorParams(other.transformablePos_, other.mass_, other.speed_)
    )
{
}

double RectangleMolecule::width()  const { return width_;  }
double RectangleMolecule::height() const { return height_; }

void   RectangleMolecule::width(double newWidth)   { width_ = newWidth;   }
void   RectangleMolecule::height(double newHeight) { height_ = newHeight; }

double RectangleMolecule::basicWidth()  { return RectangleMolecule::basicWidth_;  }
double RectangleMolecule::basicHeight() { return RectangleMolecule::basicHeight_; }

void RectangleMolecule::basicWidth (const double newWidth)  { RectangleMolecule::basicWidth_  = newWidth;  }
void RectangleMolecule::basicHeight(const double newHeight) { RectangleMolecule::basicHeight_ = newHeight; }

} // namespace Model