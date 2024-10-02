#include "Model/Molecule.hpp"

#include <iostream>
#include <cstdio>

#include "Utils/Exceptions.hpp"

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
    if (!std::isfinite(mass))
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidMass,
            "mass have to be finite",
            nullptr
        );
    }
}

Molecule::Molecule(const Molecule::CtorParams& ctorParams) : 
    Molecule(ctorParams.topLeft, ctorParams.mass, ctorParams.speed)
{
}

Molecule::Molecule(const Molecule& other) : Molecule(other.transformablePos_, other.mass_, other.speed_)
{
}

double        Molecule::mass() const                   { return mass_;       }
void          Molecule::mass(const double initMass)    
{
    if (!std::isfinite(initMass))
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidMass,
            "mass have to be finite",
            nullptr
        );
    }

    mass_ = initMass;   
}

const Vector& Molecule::speed() const &                { return speed_;      }
void          Molecule::speed(const Vector& initSpeed) { speed_ = initSpeed; }

MoleculeType Molecule::id() const { return id_; };

const Collider* Molecule::collider() const & { return collider_.get(); };

Point Molecule::topLeft() const { return transformablePos_; };

double Molecule::basicSpeedAbs() { return Molecule::basicSpeedAbs_; }
double Molecule::basicMass()     { return Molecule::basicMass_;     }

void Molecule::basicSpeedAbs(const double newSpeed) 
{
    if (!std::isfinite(newSpeed))
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidSpeedAbs,
            "basic absolute speed have to be finite",
            nullptr
        );
    }

    Molecule::basicSpeedAbs_ = newSpeed;
}
void Molecule::basicMass    (const double newMass)
{ 
    if (!std::isfinite(newMass))
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidMass,
            "basic mass have to be finite",
            nullptr
        );
    }
    
    Molecule::basicMass_ = newMass;  
}

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

double CircleMolecule::radius() const { return radius_; }

void   CircleMolecule::radius(double newRadius)
{ 
    if (!std::isfinite(newRadius) || newRadius < 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidSize,
            "radius have to be finite or >= 0",
            nullptr
        );
    }

    radius_ = newRadius; 
}

double CircleMolecule::basicRadius() { return CircleMolecule::basicRadius_; }

void CircleMolecule::basicRadius(const double newRadius)
{ 
    if (!std::isfinite(newRadius) || newRadius < 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidSize,
            "basic radius have to be finite and >= 0",
            nullptr
        );
    }

    CircleMolecule::basicRadius_ = newRadius;
}

RectangleMolecule::RectangleMolecule(
    const double width, const double height, const Molecule::CtorParams& ctorParams
) : Molecule{ctorParams}, width_(width), height_(height)
{
    if (!std::isfinite(width) || !std::isfinite(height) || width < 0 || height < 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidSize,
            "width and height have to be finite and >= 0",
            nullptr
        );
    }

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

void   RectangleMolecule::width(double newWidth)
{ 
    if (!std::isfinite(newWidth) || newWidth < 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidSize,
            "width have to be finite and >= 0",
            nullptr
        );
    }

    width_ = newWidth;
}
void   RectangleMolecule::height(double newHeight)
{
    if (!std::isfinite(newHeight) || newHeight < 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidSize,
            "height have to be finite and >= 0",
            nullptr
        );
    }

    height_ = newHeight; 
}

double RectangleMolecule::basicWidth()  { return RectangleMolecule::basicWidth_;  }
double RectangleMolecule::basicHeight() { return RectangleMolecule::basicHeight_; }

void RectangleMolecule::basicWidth (const double newWidth)
{
    if (!std::isfinite(newWidth) || newWidth < 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidSize,
            "width have to be finite and >= 0",
            nullptr
        );
    }

    RectangleMolecule::basicWidth_  = newWidth; 
}

void RectangleMolecule::basicHeight(const double newHeight)
{
    if (!std::isfinite(newHeight) || newHeight < 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidSize,
            "height have to be finite and >= 0",
            nullptr
        );
    }

    RectangleMolecule::basicHeight_ = newHeight;
}

} // namespace Simulator