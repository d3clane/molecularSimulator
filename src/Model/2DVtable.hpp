#ifndef MODEL_2D_VTABLE_HPP
#define MODEL_2D_VTABLE_HPP

#include "Model/Molecule.hpp"
#include "Model/Collider.hpp"
#include "Model/Physics.hpp"
#include "Model/Chemistry.hpp"
#include "Model/MoleculeManager.hpp"

namespace Model
{

namespace Molecules2DVtable
{

struct Molecules2DVtableFuncs
{
    CheckCollisionFuncType   checkCollision;
    PhysicsCollisionFuncType processPhysics;
    ChemistryFuncType        processChemistry;
};

void addCheckCollisions(
    const CheckCollisionFuncType (&funcs)[numberOfDifferentMolecules][numberOfDifferentMolecules]
);
void addProcessPhysics(
    const PhysicsCollisionFuncType (&funcs)[numberOfDifferentMolecules][numberOfDifferentMolecules]
);
void addProcessChemistry(
    const ChemistryFuncType (&funcs)[numberOfDifferentMolecules][numberOfDifferentMolecules]
);

bool checkCollision  (const Molecule* molecule1, const Molecule* molecule2);
bool checkCollision  (const Molecule* molecule, const Boundary* boundary);
void processPhysics  (Molecule* molecule1, Molecule* molecule2);

MoleculesAfterChemistryReaction processChemistry(Molecule* molecule1, Molecule* molecule2);

} // namespace Molecules2DVtable

} // namespace Model

#endif // MODEL_2D_VTABLE_HPP