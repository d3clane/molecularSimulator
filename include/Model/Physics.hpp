#ifndef MODEL_MODEL_PHYSICS_HPP
#define MODEL_MODEL_PHYSICS_HPP

#include <memory>

#include "Model/Molecule.hpp"

namespace Simulator
{

class Boundary;

using PhysicsCollisionFuncType  = void (*)(Molecule* molecule1, Molecule* molecule2);
using BoundaryCollisionFuncType = void (*)(Molecule* molecule, const Boundary* boundary);


void reorderEnergy(std::vector<Molecule* >& moleculesBefore, std::vector<Molecule* >& moleculesAfter);

void processCollisionCircleBoundary(Molecule* molecule, const Boundary* boundary);
void processCollisionRectBoundary  (Molecule* molecule, const Boundary* boundary);

void processCollisionCircleCircle(Molecule* molecule1, Molecule* molecule2);
void processCollisionCircleRect  (Molecule* molecule1, Molecule* molecule2);
void processCollisionRectCircle  (Molecule* molecule1, Molecule* molecule2);
void processCollisionRectRect    (Molecule* molecule1, Molecule* molecule2);

} // namespace Model


#endif // MODEL_MODEL_PHYSICS_HPP