#include "Model/Collider.hpp"
#include "Model/MoleculeManager.hpp"

#include "src/Model/2DVtable.hpp"

#include <iostream>

namespace Model
{

namespace 
{

struct MoleculesChanges
{
    std::vector<ListIterator<std::unique_ptr<Molecule> > > moleculesToDeleteIts;
    std::vector<Molecule*> moleculesToAdd;
};

MoleculesChanges processMoleculesInteraction(
    const ListIterator<std::unique_ptr<Molecule> >& firstMoleculeIt, 
    const ListIterator<std::unique_ptr<Molecule> >& secondMoleculeIt
)
{
    if (!Molecules2DVtable::checkCollision(firstMoleculeIt->get(), secondMoleculeIt->get()))
        return {};
    
    MoleculesAfterChemistryReaction afterReaction = 
        Molecules2DVtable::processChemistry(firstMoleculeIt->get(), secondMoleculeIt->get());
    
    if (!afterReaction.reacted)
    {
        Molecules2DVtable::processPhysics(firstMoleculeIt->get(), secondMoleculeIt->get());
        return {};
    }

    std::vector<Molecule* > beforeReaction = {firstMoleculeIt->get(), secondMoleculeIt->get()};

    reorderEnergy(beforeReaction, afterReaction.moleculesAfterReaction);

    std::vector<ListIterator<std::unique_ptr<Molecule> > > moleculesToDeleteIts = 
        {firstMoleculeIt, secondMoleculeIt};
    
    return {moleculesToDeleteIts, afterReaction.moleculesAfterReaction};
}

void handleCollisionBetweenMolecules(ListType<std::unique_ptr<Molecule> >& molecules)
{
    std::vector<MoleculesChanges> moleculesChanges;

    for (auto firstMoleculeIt = molecules.begin(), itEnd = molecules.end(); 
        firstMoleculeIt != itEnd; ++firstMoleculeIt)
    {
        auto secondMoleculeIt = firstMoleculeIt;
        ++secondMoleculeIt;
        
        for (; secondMoleculeIt != itEnd; ++secondMoleculeIt)
        {
            moleculesChanges.push_back(processMoleculesInteraction(firstMoleculeIt, secondMoleculeIt));
        }
    }

    for (auto& changes : moleculesChanges)
    {
        for (auto& toDelete : changes.moleculesToDeleteIts)
            molecules.erase(toDelete);
        
        for (auto& toAdd : changes.moleculesToAdd)
            molecules.push_back(std::unique_ptr<Molecule>(toAdd));
    }
}

void handleCollisionWithBoundaries(
    ListType<std::unique_ptr<Molecule> >& molecules, ListType<Boundary> boundaries
)
{
    for (auto& molecule : molecules)
    {
        for (auto& boundary : boundaries)
        {
            if (Molecules2DVtable::checkCollision(molecule.get(), &boundary))
                bounceFromBoundary(molecule.get(), &boundary);
        }
    }
}

} // namespace anon

void MoleculeManager::moveMolecules()
{
    for (auto& molecule : molecules_)
    {
       molecule->move(molecule->speed()); 
    }

    handleCollisionWithBoundaries(molecules_, boundaries_);

    handleCollisionBetweenMolecules(molecules_);
}

Boundary::Boundary(
    const Point& topLeft, const double width, const double height,
    const Vector& perpendicular
) : Engine::Transformable(topLeft), width_(width), height_(height), perpendicular_(perpendicular),
    collider_(new RectangleCollider(&transformablePos_, &width_, &height_))
{
}

Boundary::Boundary(const Boundary& other) : 
    Boundary(other.transformablePos_, other.width_, other.height_, other.perpendicular_)
{
}

const RectangleCollider* Boundary::collider() const & { return collider_.get(); }
const Vector& Boundary::perpendicular() const &       { return perpendicular_; }

} // namespace Model
