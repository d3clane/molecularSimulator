#include "Model/Molecule.hpp"

#include "src/Model/2DVtable.hpp"

namespace Simulator
{

namespace Molecules2DVtable
{

const static Molecules2DVtableFuncs 
    MoleculesCollisionsVTable[numberOfDifferentMolecules][numberOfDifferentMolecules] = 
{
    [(size_t)MoleculeType::Circle][(size_t)MoleculeType::Circle] = 
         {checkCollisionCircleCircle, processCollisionCircleCircle, processChemistryCircleCircle},

    [(size_t)MoleculeType::Circle][(size_t)MoleculeType::Rectangle] = 
        {checkCollisionCircleRect,   processCollisionCircleRect,   processChemistryCircleRect},

    [(size_t)MoleculeType::Rectangle][(size_t)MoleculeType::Circle] =
        {checkCollisionRectCircle,   processCollisionRectCircle,   processChemistryRectCircle},
    
    [(size_t)MoleculeType::Rectangle][(size_t)MoleculeType::Rectangle] =
        {checkCollisionRectRect,     processCollisionRectRect,     processChemistryRectRect},
};

#if 0
void addCheckCollisions(const CheckCollisionFuncType (&funcs)[numberOfDifferentMolecules][numberOfDifferentMolecules])
{
    for (size_t i = 0; i < numberOfDifferentMolecules; ++i)
    {
        for (size_t j = 0; j < numberOfDifferentMolecules; ++j)
        {
            MoleculesCollisionsVTable[i][j].checkCollision = funcs[i][j];
        }
    }
}

void addProcessPhysics(
    const PhysicsCollisionFuncType (&funcs)[numberOfDifferentMolecules][numberOfDifferentMolecules]
)
{
    for (size_t i = 0; i < numberOfDifferentMolecules; ++i)
    {
        for (size_t j = 0; j < numberOfDifferentMolecules; ++j)
        {
            MoleculesCollisionsVTable[i][j].processPhysics = funcs[i][j];
        }
    }
}

void addProcessChemistry(
    const ChemistryFuncType (&funcs)[numberOfDifferentMolecules][numberOfDifferentMolecules]
)
{
    for (size_t i = 0; i < numberOfDifferentMolecules; ++i)
    {
        for (size_t j = 0; j < numberOfDifferentMolecules; ++j)
        {
            MoleculesCollisionsVTable[i][j].processChemistry = funcs[i][j];
        }
    }
}
#endif

bool checkCollision  (const Molecule* molecule1, const Molecule* molecule2)
{
    assert(molecule1);
    assert(molecule2);

    return MoleculesCollisionsVTable[(size_t)molecule1->id()][(size_t)molecule2->id()].checkCollision(
        molecule1->collider(), molecule2->collider()
    );
}

bool checkCollision  (const Molecule* molecule, const Boundary* boundary)
{
    assert(molecule);
    assert(boundary);

    return MoleculesCollisionsVTable[(size_t)molecule->id()][(size_t)MoleculeType::Rectangle].checkCollision(
        molecule->collider(), boundary->collider() 
    );
}

void processPhysics  (Molecule* molecule1, Molecule* molecule2)
{
    assert(molecule1);
    assert(molecule2);

    MoleculesCollisionsVTable[(size_t)molecule1->id()][(size_t)molecule2->id()].processPhysics(molecule1, molecule2);
}

MoleculesAfterChemistryReaction processChemistry(Molecule* molecule1, Molecule* molecule2)
{
    assert(molecule1);
    assert(molecule2);

    return MoleculesCollisionsVTable[(size_t)molecule1->id()][(size_t)molecule2->id()].processChemistry(
        molecule1, molecule2
    );
}

} // namespace Molecules2DVtable

} // namespace Simulator
