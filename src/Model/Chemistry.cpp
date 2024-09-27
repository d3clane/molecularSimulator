#include "Model/Chemistry.hpp"

namespace Model
{

MoleculesAfterChemistryReaction processChemistryRectRect(Molecule* rectMolecule1, Molecule* rectMolecule2)
{
    RectangleMolecule& molecule1 = *dynamic_cast<RectangleMolecule*>(rectMolecule1);
    RectangleMolecule& molecule2 = *dynamic_cast<RectangleMolecule*>(rectMolecule2);

    MoleculesAfterChemistryReaction result;
    result.reacted = true;

    RectangleMolecule* resultMolecule = new RectangleMolecule(molecule1);

    resultMolecule->mass(molecule1.mass() + molecule2.mass());
    resultMolecule->speed(
        (molecule1.speed() * molecule1.mass() + molecule2.speed() * molecule2.mass()) / resultMolecule->mass()
    );
    
    result.moleculesAfterReaction.push_back(resultMolecule);

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

    double criticalReactionImpulseValue = 1;

    MoleculesAfterChemistryReaction result;
    if (impulseValue < criticalReactionImpulseValue)
    {
        result.reacted = false;
        return result;
    }

    result.reacted = true;
    
    double newMass   = molecule1.mass()   + molecule2.mass();
    double newHeight = molecule1.radius() + molecule2.radius();
    
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

    double newMass  = rectMolecule.mass() + circleMolecule.mass();
    double newSize  = rectMolecule.height() * newMass / rectMolecule.mass();
    Vector newSpeed = 
        (rectMolecule.speed() * rectMolecule.mass() + circleMolecule.speed() * circleMolecule.mass()) / newMass;

    RectangleMolecule* resultMolecule = new RectangleMolecule(rectMolecule);

    resultMolecule->mass  (newMass);
    resultMolecule->speed (newSpeed);
    resultMolecule->height(newSize);
    resultMolecule->width (newSize);

    result.moleculesAfterReaction.push_back(resultMolecule);

    return result;
}

MoleculesAfterChemistryReaction processChemistryCircleRect(Molecule* molecule1, Molecule* molecule2)
{
    return processChemistryRectCircle(molecule2, molecule1);
}

} // namespace Model