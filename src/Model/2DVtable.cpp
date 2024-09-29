#include "Model/Molecule.hpp"

#include "src/Model/2DVtable.hpp"

namespace Simulator
{

namespace Molecules2DVtable
{

// TODO: will broke when changing molecule type id's, should use enum
static Molecules2DVtableFuncs 
    MoleculesCollisionsVTable[numberOfDifferentMolecules][numberOfDifferentMolecules] = 
{
    {
        {checkCollisionCircleCircle, processCollisionCircleCircle, processChemistryCircleCircle},
        {checkCollisionCircleRect,   processCollisionCircleRect,   processChemistryCircleRect},
    },
    {
        {checkCollisionRectCircle,   processCollisionRectCircle,   processChemistryRectCircle},
        {checkCollisionRectRect,     processCollisionRectRect,     processChemistryRectRect},
    }
};

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

bool checkCollision  (const Molecule* molecule1, const Molecule* molecule2)
{
    return MoleculesCollisionsVTable[(size_t)molecule1->id()][(size_t)molecule2->id()].checkCollision(
        molecule1->collider(), molecule2->collider()
    );
}

bool checkCollision  (const Molecule* molecule, const Boundary* boundary)
{
    return MoleculesCollisionsVTable[(size_t)molecule->id()][(size_t)MoleculeType::Rectangle].checkCollision(
        molecule->collider(), boundary->collider() 
    );
}

void processPhysics  (Molecule* molecule1, Molecule* molecule2)
{
    MoleculesCollisionsVTable[(size_t)molecule1->id()][(size_t)molecule2->id()].processPhysics(molecule1, molecule2);
}

MoleculesAfterChemistryReaction processChemistry(Molecule* molecule1, Molecule* molecule2)
{
    return MoleculesCollisionsVTable[(size_t)molecule1->id()][(size_t)molecule2->id()].processChemistry(
        molecule1, molecule2
    );
}

} // namespace Molecules2DVtable

} // namespace Simulator
