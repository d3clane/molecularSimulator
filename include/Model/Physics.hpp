#ifndef MODEL_MODEL_PHYSICS_HPP
#define MODEL_MODEL_PHYSICS_HPP

#include <memory>

#include "Model/Molecule.hpp"

namespace Simulator
{

using PhysicsCollisionFuncType = void (*)(Molecule* molecule1, Molecule* molecule2);

class Boundary;

void reorderEnergy(std::vector<Molecule* >& moleculesBefore, std::vector<Molecule* >& moleculesAfter);

void bounceFromBoundary(Molecule* molecule, const Boundary* boundary);

void processCollisionCircleCircle(Molecule* molecule1, Molecule* molecule2);
void processCollisionCircleRect  (Molecule* molecule1, Molecule* molecule2);
void processCollisionRectCircle  (Molecule* molecule1, Molecule* molecule2);
void processCollisionRectRect    (Molecule* molecule1, Molecule* molecule2);

} // namespace Model


#endif // MODEL_MODEL_PHYSICS_HPP