#include "Model/Physics.hpp"
#include "Model/MoleculeManager.hpp"

#include "Utils/Rand.hpp"
#include "Utils/Exceptions.hpp"

#include <iostream>

namespace Simulator
{

namespace 
{

void setSpeedsAfterCollision(CircleMolecule& molecule1, CircleMolecule& molecule2)
{
    Vector aSpeed = molecule1.speed();
    Vector bSpeed = molecule2.speed();

    unsigned int aMass = molecule1.mass();
    unsigned int bMass = molecule2.mass();

    Vector relativeASpeedBefore = aSpeed - bSpeed;

    Vector centerDirection      = Vector(molecule1.topLeft(), molecule2.topLeft());
    if (Engine::cos(centerDirection, relativeASpeedBefore) < 0) // problem - they went through each other
        centerDirection = -centerDirection;

    Vector centerDirectionSpeed = relativeASpeedBefore.projectOnto(centerDirection);

    double v_x = centerDirectionSpeed.length();

    double relativeBSpeedAfterLen = 2 * aMass * bMass * v_x / (aMass * bMass + bMass * bMass);

    Vector relativeBSpeedAfter = centerDirection.getNormalizedVector() * relativeBSpeedAfterLen;
    Vector relativeASpeedAfter = relativeASpeedBefore - (bMass / aMass) * relativeBSpeedAfter;

    Vector aSpeedAfter = bSpeed + relativeASpeedAfter;
    Vector bSpeedAfter = bSpeed + relativeBSpeedAfter;
    
    molecule1.speed(aSpeedAfter);
    molecule2.speed(bSpeedAfter);
}

void resolveOverlap(CircleMolecule& molecule1, CircleMolecule& molecule2)
{
    float overlapping = molecule1.radius() + molecule2.radius() - 
                        Engine::getDistance2D(molecule1.topLeft(), molecule2.topLeft());

    if (overlapping <= 0) return;

    Vector direction1 = molecule1.speed().getNormalizedVector();
    Vector direction2 = molecule2.speed().getNormalizedVector();

    Vector standardDirection = Vector{Utils::Rand(-1, 1), Utils::Rand(-1, 1), 0}.getNormalizedVector();

    direction1 = direction1.isZero() ?  standardDirection : direction1;
    direction2 = direction2.isZero() ? -standardDirection : direction2;

    direction1 = (direction1 == direction2 ? -direction1 : direction1);

    molecule1.move(direction1 * overlapping / 2);
    molecule2.move(direction2 * overlapping / 2);
}

void bounceFromBoundary(Molecule* molecule, const Boundary* boundary) 
{
    Vector speed = molecule->speed();

    Vector newSpeed = -speed.reflectRelatively(boundary->perpendicular());

    molecule->speed(newSpeed);
}

void resolveOverlap(CircleMolecule& molecule, const Boundary& boundary)
{
    double radius = molecule.radius();
    Point center = molecule.topLeft() + Vector(radius, radius, 0);

    assert(boundary.perpendicular() == boundary.perpendicular().getNormalizedVector());

    // TODO: doesn't understand width of the boundary

    float overlap = radius - (-boundary.perpendicular() ^ Vector(center, boundary.pos()));

    if (overlap <= 0) return;

    molecule.move(boundary.perpendicular() * overlap);
}

void resolveOverlap(RectangleMolecule& molecule, const Boundary& boundary)
{
    double height = molecule.height();
    double width  = molecule.width();

    Point center = molecule.topLeft() + Vector(width / 2, height / 2, 0);

    // TODO: works only in cases (-1, 0), (1, 0), (0, -1), (0, 1) perpendiculars
    double criticalDistance = std::abs(Vector(width / 2, height / 2, 0) ^ boundary.perpendicular());

    double overlap = criticalDistance - (-boundary.perpendicular() ^ Vector(center, boundary.pos()));

    if (overlap <= 0) return;

    molecule.move(boundary.perpendicular() * overlap);
}

} // namespace anon

void reorderEnergy(std::vector<Molecule* >& moleculesBefore, std::vector<Molecule* >& moleculesAfter)
{
    if (moleculesAfter.size() == 0 || moleculesBefore.size() == 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::PhysicsErr,
            "can't reorder energy when there is zero molecules in one of the cases",
            nullptr
        );
    }

    double energy = 0;
    Vector impulse{0, 0, 0};

    for (auto& molecule : moleculesBefore)
    {
        double speed = molecule->speed().length();
        energy += molecule->mass() * speed * speed;

        impulse += molecule->mass() * molecule->speed();
    }

    energy /= 2;

    double energyForOne = energy / moleculesAfter.size();

    // TODO: law of conservation of momentum
    for (auto& molecule : moleculesAfter)
    {
        double myImpulse = std::sqrt(energyForOne * 2 * molecule->mass());
        double mySpeed = myImpulse / molecule->mass();

        double vXCoeff = Utils::Rand(-1, 1);
        double vyCoeff = std::sqrt(1 - vXCoeff * vXCoeff) * Utils::RandDirection();

        molecule->speed({mySpeed * vXCoeff, mySpeed * vyCoeff, 0});
    }
}

void reorderImpulse(std::vector<Molecule* >& moleculesBefore, Molecule* moleculeAfter)
{
    Vector impulse{0, 0, 0};

    for (auto& molecule : moleculesBefore)
    {
        impulse += molecule->speed() * molecule->mass();
    }

    moleculeAfter->speed(impulse / moleculeAfter->mass());
}

void processCollisionCircleBoundary(Molecule* molecule, const Boundary* boundary)
{
    bounceFromBoundary(molecule, boundary);

    CircleMolecule& circleMolecule = *static_cast<CircleMolecule*>(molecule);
    
    resolveOverlap(circleMolecule, *boundary);
}

void processCollisionRectBoundary(Molecule* molecule, const Boundary* boundary)
{
    bounceFromBoundary(molecule, boundary);

    RectangleMolecule& rectangleMolecule = *static_cast<RectangleMolecule*>(molecule);

    resolveOverlap(rectangleMolecule, *boundary);
}

void processCollisionCircleCircle(Molecule* circleMolecule1, Molecule* circleMolecule2)
{
    CircleMolecule& molecule1 = *static_cast<CircleMolecule*>(circleMolecule1);
    CircleMolecule& molecule2 = *static_cast<CircleMolecule*>(circleMolecule2);
    
    setSpeedsAfterCollision(molecule1, molecule2);
    resolveOverlap(molecule1, molecule2);
}

void processCollisionCircleRect  (Molecule* molecule1, Molecule* molecule2)
{
    assert(false);
    return; // Impossible in my model
}

void processCollisionRectCircle  (Molecule* molecule1, Molecule* molecule2)
{
    assert(false);
    return; // Impossible in my model
}

void processCollisionRectRect    (Molecule* molecule1, Molecule* molecule2)
{
    //assert(false);
    return; // Impossible in my model
}

} // namespace Simulator