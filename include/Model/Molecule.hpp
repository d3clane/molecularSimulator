#ifndef MODEL_MOLECULE_HPP
#define MODEL_MOLECULE_HPP

#include "Graphics/Sprite.hpp"

#include "Model/Collider.hpp"

#include "Engine/Transformable.hpp"
#include "Engine/CoordsSystem.hpp"
#include "Engine/Vector.hpp"

#include <memory>

namespace Simulator
{

using Engine::Point;
using Engine::Vector;

static const size_t numberOfDifferentMolecules = 2;
enum class MoleculeType
{
    Circle,
    Rectangle,
};

class Molecule : public Engine::Transformable
{
    static double basicSpeedAbs_;
    static double basicMass_;

protected:
    MoleculeType id_;

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

    MoleculeType id() const;

    const Collider* collider() const &;

    const Vector& speed() const &;
    void          speed(const Vector& speed);

    Point topLeft() const;

    static double basicSpeedAbs();
    static double basicMass();

    static void basicSpeedAbs(const double newSpeedAbs);
    static void basicMass    (const double newMass);
};

class CircleMolecule : public Molecule
{
    static double basicRadius_;

    double radius_;

public:
    CircleMolecule(const double radius, const Molecule::CtorParams& ctorParams);
    CircleMolecule(const CircleMolecule&  other);
    CircleMolecule(CircleMolecule&& other);

    double radius() const;
    void   radius(double newRadius);

    static double basicRadius();

    static void basicRadius(const double newRadius);
};

class RectangleMolecule : public Molecule
{
    static double basicWidth_;
    static double basicHeight_;

    double width_;
    double height_;

public:
    RectangleMolecule(const double width, const double height, const Molecule::CtorParams& ctorParams);
    RectangleMolecule(const RectangleMolecule& other);

    double width()  const;
    void   width(double newWidth);
    double height() const;
    void   height(double newHeight);

    static double basicWidth();
    static double basicHeight();

    static void basicWidth(const double newWidth);
    static void basicHeight(const double newHeight);
};

} // namespace Model

#endif // MODEL_MOLECULE_HPP