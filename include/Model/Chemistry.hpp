#ifndef MODEL_CHEMISTRY_HPP
#define MODEL_CHEMISTRY_HPP

#include <vector>
#include <memory>

#include "Model/Molecule.hpp"

namespace Simulator
{

struct MoleculesAfterChemistryReaction
{
    bool reacted;
    std::vector<Molecule*> moleculesAfterReaction;
};

using ChemistryFuncType = MoleculesAfterChemistryReaction (*)(Molecule* molecule1, Molecule* molecule2);

MoleculesAfterChemistryReaction processChemistryRectRect    (Molecule* molecule1, Molecule* molecule2);
MoleculesAfterChemistryReaction processChemistryCircleCircle(Molecule* molecule1, Molecule* molecule2);
MoleculesAfterChemistryReaction processChemistryCircleRect  (Molecule* molecule1, Molecule* molecule2);
MoleculesAfterChemistryReaction processChemistryRectCircle  (Molecule* molecule1, Molecule* molecule2);

} // namespace Simulator

#endif // MODEL_CHEMISTRY_HPP