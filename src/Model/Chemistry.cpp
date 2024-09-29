#include "Model/Chemistry.hpp"

#include <iostream>

namespace Simulator
{

MoleculesAfterChemistryReaction processChemistryRectRect(Molecule* rectMolecule1, Molecule* rectMolecule2)
{
    RectangleMolecule& molecule1 = *dynamic_cast<RectangleMolecule*>(rectMolecule1);
    RectangleMolecule& molecule2 = *dynamic_cast<RectangleMolecule*>(rectMolecule2);

    MoleculesAfterChemistryReaction result; result.reacted = false; return result;
    result.reacted = true;

    size_t numberOfMolecules = static_cast<size_t>(molecule1.mass() + molecule2.mass());
    Point topLeft = molecule1.topLeft();

    for (size_t i = 0; i < numberOfMolecules; ++i)
    {
        CircleMolecule* newMolecule = new CircleMolecule{
            CircleMolecule::basicRadius(),
            Molecule::CtorParams{topLeft, CircleMolecule::basicMass(), Vector{0, 0, 0}}
        };

        result.moleculesAfterReaction.push_back(newMolecule);
        //topLeft += Engine::Vector{20, 0, 0};
    }

    return result;
}

MoleculesAfterChemistryReaction processChemistryCircleCircle(
    Molecule* circleMolecule1, Molecule* circleMolecule2
)
{
    CircleMolecule& molecule1 = *dynamic_cast<CircleMolecule*>(circleMolecule1);
    CircleMolecule& molecule2 = *dynamic_cast<CircleMolecule*>(circleMolecule2);

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
        result.reacted = false;
        return result;
    }

    result.reacted = true;
    
    double rectangleBasicMass = RectangleMolecule::basicMass();

    double newMass   = molecule1.mass() + molecule2.mass();
    double newHeight = RectangleMolecule::basicHeight() * newMass / rectangleBasicMass;
    double newWidth  = RectangleMolecule::basicWidth()  * newMass / rectangleBasicMass;
    
    Vector newSpeed = 
        (molecule1.speed() * molecule1.mass() + molecule2.speed() * molecule2.mass()) / newMass;
    
    RectangleMolecule* resultMolecule = new RectangleMolecule(
        newHeight, newHeight, Molecule::CtorParams{molecule1.topLeft(), newMass, newSpeed}
    );

    result.moleculesAfterReaction.push_back(resultMolecule);

    return result;
}

MoleculesAfterChemistryReaction processChemistryRectCircle(Molecule* molecule1, Molecule* molecule2)
{
    RectangleMolecule& rectMolecule   = *dynamic_cast<RectangleMolecule*>(molecule1);
    CircleMolecule&    circleMolecule = *dynamic_cast<CircleMolecule*>   (molecule2);

    MoleculesAfterChemistryReaction result;
    result.reacted = true;

    double basicMass = RectangleMolecule::basicMass();

    double newMass  = rectMolecule.mass() + circleMolecule.mass();
    double newHeight = RectangleMolecule::basicHeight() * newMass / basicMass;
    double newWidth  = RectangleMolecule::basicWidth()  * newMass / basicMass;

    Vector newSpeed = 
        (rectMolecule.speed() * rectMolecule.mass() + circleMolecule.speed() * circleMolecule.mass()) / newMass;

    RectangleMolecule* resultMolecule = new RectangleMolecule(rectMolecule);

    resultMolecule->mass  (newMass);
    resultMolecule->speed (newSpeed);
    resultMolecule->height(newHeight);
    resultMolecule->width (newWidth);

    result.moleculesAfterReaction.push_back(resultMolecule);

    return result;
}

MoleculesAfterChemistryReaction processChemistryCircleRect(Molecule* molecule1, Molecule* molecule2)
{
    return processChemistryRectCircle(molecule2, molecule1);
}

} // namespace Simulator