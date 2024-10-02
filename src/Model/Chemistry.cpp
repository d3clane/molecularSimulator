#include "Model/Chemistry.hpp"

#include <iostream>

namespace Simulator
{

namespace 
{

double getNewRectMoleculeSize(const double mass, const double basicMass, const double basicWidth)
{
    return std::sqrt(mass / basicMass) * basicWidth;
}

} // namespace anon

MoleculesAfterChemistryReaction processChemistryRectRect(Molecule* rectMolecule1, Molecule* rectMolecule2)
{
    assert(rectMolecule1);
    assert(rectMolecule2);

    RectangleMolecule& molecule1 = *static_cast<RectangleMolecule*>(rectMolecule1);
    RectangleMolecule& molecule2 = *static_cast<RectangleMolecule*>(rectMolecule2);

    MoleculesAfterChemistryReaction result;
    result.requiredPhysics = RequiredPhysics::ReorderEnergy;

    size_t numberOfMolecules = static_cast<size_t>(molecule1.mass() + molecule2.mass());
    Point topLeft = molecule1.topLeft();

    const double basicRadius = CircleMolecule::basicRadius();
    for (size_t i = 0; i < numberOfMolecules; ++i)
    {
        CircleMolecule* newMolecule = new CircleMolecule{
            basicRadius, Molecule::CtorParams{topLeft, CircleMolecule::basicMass(), Vector{0, 0, 0}}
        };

        result.moleculesAfterReaction.push_back(newMolecule);
        topLeft += Engine::Vector{2 * basicRadius, 0, 0};
    }

    return result;
}

MoleculesAfterChemistryReaction processChemistryCircleCircle(
    Molecule* circleMolecule1, Molecule* circleMolecule2
)
{
    assert(circleMolecule1);
    assert(circleMolecule2);

    CircleMolecule& molecule1 = *static_cast<CircleMolecule*>(circleMolecule1);
    CircleMolecule& molecule2 = *static_cast<CircleMolecule*>(circleMolecule2);

    Vector centerLine = Vector(molecule1.topLeft(), molecule2.topLeft());

    Vector impulse = molecule1.speed().projectOnto(centerLine) * molecule1.mass() -
                     molecule2.speed().projectOnto(centerLine) * molecule2.mass();


    double impulseValue = impulse.length();

    double basicMass     = CircleMolecule::basicMass();
    double basicSpeedAbs = CircleMolecule::basicSpeedAbs();

    double criticalReactionImpulseValue = basicMass * basicSpeedAbs * 1.7;

    MoleculesAfterChemistryReaction result;
    if (impulseValue < criticalReactionImpulseValue)
    {
        result.requiredPhysics = RequiredPhysics::Nothing;
        return result;
    }

    result.requiredPhysics = RequiredPhysics::ReorderImpulse;
    
    double rectangleBasicMass = RectangleMolecule::basicMass();

    double newMass   = molecule1.mass() + molecule2.mass();
    double newHeight = getNewRectMoleculeSize(newMass, basicMass, RectangleMolecule::basicHeight());
    double newWidth  = getNewRectMoleculeSize(newMass, basicMass, RectangleMolecule::basicWidth ());
    
    RectangleMolecule* resultMolecule = new RectangleMolecule(
        newHeight, newHeight, Molecule::CtorParams{molecule1.topLeft(), newMass, {0, 0, 0}}
    );

    result.moleculesAfterReaction.push_back(resultMolecule);

    return result;
}

MoleculesAfterChemistryReaction processChemistryRectCircle(Molecule* molecule1, Molecule* molecule2)
{
    assert(molecule1);
    assert(molecule2);

    RectangleMolecule& rectMolecule   = *static_cast<RectangleMolecule*>(molecule1);
    CircleMolecule&    circleMolecule = *static_cast<CircleMolecule*>   (molecule2);

    MoleculesAfterChemistryReaction result;
    result.requiredPhysics = RequiredPhysics::ReorderImpulse;

    double basicMass = RectangleMolecule::basicMass();

    double newMass   = rectMolecule.mass() + circleMolecule.mass();
    double newHeight = getNewRectMoleculeSize(newMass, basicMass, RectangleMolecule::basicHeight());
    double newWidth  = getNewRectMoleculeSize(newMass, basicMass, RectangleMolecule::basicWidth ());

    auto* resultMolecule = new RectangleMolecule(rectMolecule);

    resultMolecule->mass  (newMass);
    resultMolecule->height(newHeight);
    resultMolecule->width (newWidth);

    result.moleculesAfterReaction.push_back(resultMolecule);

    return result;
}

MoleculesAfterChemistryReaction processChemistryCircleRect(Molecule* molecule1, Molecule* molecule2)
{
    assert(molecule1);
    assert(molecule2);
    
    return processChemistryRectCircle(molecule2, molecule1);
}

} // namespace Simulator
