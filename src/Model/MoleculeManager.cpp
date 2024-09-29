#include "Model/Collider.hpp"
#include "Model/MoleculeManager.hpp"

#include "src/Model/2DVtable.hpp"

#include "Utils/Rand.hpp"

#include <iostream>
#include <algorithm>
#include <list>

namespace Simulator
{

namespace 
{

struct MoleculesChanges
{
    std::vector<ListIterator<std::unique_ptr<Molecule> > > moleculesToDeleteIts;
    std::vector<Molecule*> moleculesToAdd;
};

struct MoleculesIteratorWrap
{
    bool isDeleted;
    ListIterator<std::unique_ptr<Molecule> > it;
};

MoleculesChanges processMoleculesInteraction(
    MoleculesIteratorWrap& firstMoleculeIt, MoleculesIteratorWrap& secondMoleculeIt
)
{
    if (!Molecules2DVtable::checkCollision(firstMoleculeIt.it->get(), secondMoleculeIt.it->get()))
        return {};

    MoleculesAfterChemistryReaction afterReaction = 
        Molecules2DVtable::processChemistry(firstMoleculeIt.it->get(), secondMoleculeIt.it->get());

    if (!afterReaction.reacted)
    {
        Molecules2DVtable::processPhysics(firstMoleculeIt.it->get(), secondMoleculeIt.it->get());
        return {};
    }

    firstMoleculeIt.isDeleted  = true;
    secondMoleculeIt.isDeleted = true;

    std::vector<Molecule* > beforeReaction = {firstMoleculeIt.it->get(), secondMoleculeIt.it->get()};

    reorderEnergy(beforeReaction, afterReaction.moleculesAfterReaction);

    std::vector<ListIterator<std::unique_ptr<Molecule> > > moleculesToDeleteIts = 
        {firstMoleculeIt.it, secondMoleculeIt.it};

    return {moleculesToDeleteIts, afterReaction.moleculesAfterReaction};
}

void handleCollisionBetweenMolecules(ListType<std::unique_ptr<Molecule> >& inputMolecules)
{
    std::vector<MoleculesChanges> moleculesChanges;

    ListType<MoleculesIteratorWrap> moleculesIts;
    
    for (auto moleculeIt = inputMolecules.begin(), itEnd = inputMolecules.end(); 
         moleculeIt != itEnd; ++moleculeIt)
    {
        moleculesIts.push_back({false, moleculeIt}); // TODO: CTOR STRUCT
    }

    for (auto firstMoleculeIt = moleculesIts.begin(), itEnd = moleculesIts.end(); 
        firstMoleculeIt != itEnd; ++firstMoleculeIt)
    {
        auto secondMoleculeIt = firstMoleculeIt;
        ++secondMoleculeIt;
        
        for (; secondMoleculeIt != itEnd; ++secondMoleculeIt)
        {
            if (firstMoleculeIt->isDeleted)  break;
            if (secondMoleculeIt->isDeleted) continue;

            moleculesChanges.push_back(processMoleculesInteraction(*firstMoleculeIt, *secondMoleculeIt));
        }
    }

    for (auto& changes : moleculesChanges)
    {
        for (auto& toDelete : changes.moleculesToDeleteIts)
            inputMolecules.erase(toDelete);
        for (auto& toAdd : changes.moleculesToAdd)
            inputMolecules.push_back(std::unique_ptr<Molecule>(toAdd));
    }
}

std::vector<BoundaryCollisionFuncType> fillBoundaryCollisions()
{
    std::vector<BoundaryCollisionFuncType> collisionFuncs(numberOfDifferentMolecules);

    collisionFuncs[(size_t)MoleculeType::Circle]    = processCollisionCircleBoundary;
    collisionFuncs[(size_t)MoleculeType::Rectangle] = processCollisionRectBoundary;

    return collisionFuncs;
}

void handleCollisionWithBoundaries(
    ListType<std::unique_ptr<Molecule> >& molecules, ListType<Boundary> boundaries
)
{
    static const std::vector<BoundaryCollisionFuncType> collisionFuncs = fillBoundaryCollisions();

    for (auto& molecule : molecules)
    {
        for (auto& boundary : boundaries)
        {
            if (Molecules2DVtable::checkCollision(molecule.get(), &boundary))
            {
                collisionFuncs[(size_t)molecule->id()](molecule.get(), &boundary);
            }
        }
    }
}

bool isOutOfBoundary(const Molecule* molecule, const Point& boundaryTopLeft, const Point& boundaryBottomRight)
{
    Point moleculePos = molecule->pos();

    const double deltaStuck = 2;

    return moleculePos.x < boundaryTopLeft.x - deltaStuck ||
           moleculePos.y < boundaryTopLeft.y - deltaStuck ||
           moleculePos.x > boundaryBottomRight.x ||
           moleculePos.y > boundaryBottomRight.y;
}

void returnOutOfBoundaries(
    ListType<std::unique_ptr<Molecule> >& molecules, 
    const Point& boundaryTopLeft, const Point& boundaryBottomRight)
{
    for (auto& molecule : molecules)
    {
        if (isOutOfBoundary(molecule.get(), boundaryTopLeft, boundaryBottomRight))
        {
            molecule.get()->pos(
                {
                    Utils::Rand(boundaryTopLeft.x, boundaryBottomRight.x),
                    Utils::Rand(boundaryTopLeft.y, boundaryBottomRight.y), 
                    0
                }
            );
        }
    }
}

std::chrono::nanoseconds calcDeltaTime(const std::chrono::steady_clock::time_point& prevTime)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - prevTime);
}

} // namespace anon

MoleculeManager::MoleculeManager(const Point& boundaryTopLeft, const Point& boundaryBottomRight) : 
    boundaryTopLeft_(boundaryTopLeft), boundaryBottomRight_(boundaryBottomRight)
{
}

void MoleculeManager::moveMolecules()
{
    
    const double millisecondsPerSecond = 1e9;
    const double deltaTime = calcDeltaTime(prevTime_).count() / millisecondsPerSecond;

    for (auto& molecule : molecules_)
    {
       molecule->move(molecule->speed() * deltaTime);
    }

    handleCollisionWithBoundaries(molecules_, boundaries_);

    handleCollisionBetweenMolecules(molecules_);

    //returnOutOfBoundaries(molecules_, boundaryTopLeft_, boundaryBottomRight_);

    prevTime_ = std::chrono::steady_clock::now();
}

void MoleculeManager::addMolecule(std::unique_ptr<Molecule> molecule)
{
    molecules_.push_back(std::move(molecule));
}

void MoleculeManager::removeMolecules(const Point& topLeft, const Point& bottomRight)
{
    molecules_.remove_if(
        [&](auto& molecule) 
        { 
            return molecule->pos().x > topLeft.x && molecule->pos().x < bottomRight.x && 
                   molecule->pos().y > topLeft.y && molecule->pos().y < bottomRight.y;
        }
    );
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

} // namespace Simulator
