#ifndef MODEL_MOLECULE_HPP
#define MODEL_MOLECULE_HPP

#include "Graphics/Sprite.hpp"

#include "Model/Collider.hpp"

#include "Engine/Transformable.hpp"
#include "Engine/CoordsSystem.hpp"
#include "Engine/Vector.hpp"

#include <memory>

namespace Model
{

using Engine::Point;
using Engine::Vector;

static const size_t numberOfDifferentMolecules = 2;
enum class MoleculeId
{
    Circle,
    Rectangle,
};

class Molecule : public Engine::Transformable
{
protected:
    MoleculeId id_;

    double mass_;
    Vector speed_;

    std::unique_ptr<Collider> collider_;  
public:
    struct CtorParams
    {
        const Point& topLeft;
        const double mass;
        const Vector& speed;

        CtorParams(const Point& topLeft, const double mass, const Vector& speed) : 
            topLeft(topLeft), mass(mass), speed(speed) {}
    };

    Molecule(const Point& topLeft, const double mass, const Vector& speed);
    Molecule(const CtorParams& ctorParams);
    Molecule(const Molecule& other);

    double mass() const;
    void   mass(const double mass);

    MoleculeId id() const;

    const Collider* collider() const &;

    const Vector& speed() const &;
    void          speed(const Vector& speed);

    Point topLeft() const;
};

class CircleMolecule : public Molecule
{
    double radius_;

public:
    CircleMolecule(const double radius, const Molecule::CtorParams& ctorParams);
    CircleMolecule(const CircleMolecule&  other);
    CircleMolecule(CircleMolecule&& other);

    double radius() const;
    void   radius(double newRadius);
};

class RectangleMolecule : public Molecule
{
    double width_;
    double height_;

public:
    RectangleMolecule(const double width, const double height, const Molecule::CtorParams& ctorParams);
    RectangleMolecule(const RectangleMolecule& other);

    double width()  const;
    void   width(double newWidth);
    double height() const;
    void   height(double newHeight);
};

} // namespace Model

#endif // MODEL_MOLECULE_HPP