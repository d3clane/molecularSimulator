#include "Model/Physics.hpp"
#include "Model/MoleculeManager.hpp"

#include "Utils/Rand.hpp"

#include <iostream>
namespace Model
{

void reorderEnergy(std::vector<Molecule* >& moleculesBefore, std::vector<Molecule* >& moleculesAfter)
{
    assert(moleculesBefore.size() > 0 && moleculesAfter.size() > 0);

    double energy = 0;
    Vector impulse{0, 0, 0};

    for (auto& molecule : moleculesBefore)
    {
        double speed = molecule->speed().length();
        energy += molecule->mass() * speed * speed;

        impulse += molecule->mass() * molecule->speed();
    }

    double energyForOne = energy / moleculesAfter.size() / 2;

    // TODO: law of conservation of momentum
    for (auto& molecule : moleculesAfter)
    {
        double myImpulse = std::sqrt(energyForOne * 2 * molecule->mass());
        double mySpeed = myImpulse / molecule->mass();

        double vXCoeff = Utils::Rand(0, 1);
        double vyCoeff = 1 - vXCoeff * vXCoeff;
        vyCoeff = vyCoeff < 0 ? 0 : std::sqrt(vyCoeff);

        molecule->speed({mySpeed * vXCoeff, mySpeed * vyCoeff, 0});
    }
}


void bounceFromBoundary(Molecule* molecule, const Boundary* boundary)
{
    Vector speed = molecule->speed();

    assert(std::isfinite(speed.length()));
    assert(std::isfinite(boundary->perpendicular().length()));
    
    Vector newSpeed = -speed.reflectRelatively(boundary->perpendicular());

    molecule->speed(newSpeed);
}

void processCollisionCircleCircle(Molecule* circleMolecule1, Molecule* circleMolecule2)
{
    CircleMolecule& a = *dynamic_cast<CircleMolecule*>(circleMolecule1);
    CircleMolecule& b = *dynamic_cast<CircleMolecule*>(circleMolecule2);

    Vector aSpeed = a.speed();
    Vector bSpeed = b.speed();

    unsigned int aMass = a.mass();
    unsigned int bMass = b.mass();

    Vector relativeASpeedBefore = aSpeed - bSpeed;

    Vector centerDirection      = Vector(a.topLeft(), b.topLeft());
    Vector centerDirectionSpeed = relativeASpeedBefore.projectOnto(centerDirection);

    double v_x = centerDirectionSpeed.length();

    double relativeBSpeedAfterLen = 2 * aMass * bMass * v_x / (aMass * bMass + bMass * bMass);

    Vector relativeBSpeedAfter = centerDirection.getNormalizedVector() * relativeBSpeedAfterLen;
    Vector relativeASpeedAfter = relativeASpeedBefore - (bMass / aMass) * relativeBSpeedAfter;

    Vector aSpeedAfter = bSpeed + relativeASpeedAfter;
    Vector bSpeedAfter = bSpeed + relativeBSpeedAfter;

    //a.move(-3 * a.speed());
    //b.move(-3 * b.speed());

    static const double alignCoeff = 1.2;
    a.speed(aSpeedAfter / alignCoeff);
    b.speed(bSpeedAfter / alignCoeff);
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

} // namespace Model