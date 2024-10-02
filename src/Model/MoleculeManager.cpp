#include "Model/Collider.hpp"
#include "Model/MoleculeManager.hpp"

#include "src/Model/2DVtable.hpp"

#include "Utils/Rand.hpp"
#include "Utils/Exceptions.hpp"

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

    MoleculesIteratorWrap(bool isDeleted, ListIterator<std::unique_ptr<Molecule> > it)
        : isDeleted(isDeleted), it(it) {}
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

    firstMoleculeIt .isDeleted = true;
    secondMoleculeIt.isDeleted = true;

    std::vector<Molecule* > beforeReaction = {firstMoleculeIt.it->get(), secondMoleculeIt.it->get()};

    // TODO: need reordering impulse, not energy in case they collide
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
        moleculesIts.push_back(MoleculesIteratorWrap{false, moleculeIt});
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

std::chrono::nanoseconds calcDeltaTime(const std::chrono::steady_clock::time_point& prevTime)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now() - prevTime
    );
}

bool isOutOfBoundary(const Molecule* molecule, const Point& boundaryTopLeft, const Point& boundaryBottomRight)
{
    assert(molecule);

    Point moleculePos = molecule->pos();

    return moleculePos.x < boundaryTopLeft.x ||
           moleculePos.y < boundaryTopLeft.y ||
           moleculePos.x > boundaryBottomRight.x ||
           moleculePos.y > boundaryBottomRight.y;
}

} // namespace anon

MoleculeManager::MoleculeManager(
    const Point& boundaryTopLeft, const Point& boundaryBottomRight
) : boundaryTopLeft_(boundaryTopLeft), boundaryBottomRight_(boundaryBottomRight)
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

    molecules_.remove_if(
        [&](auto& molecule)
        {
            return isOutOfBoundary(molecule.get(), topLeft, bottomRight);
        }
    );
}

double MoleculeManager::getTemperature() const
{
    double energy = 0;

    for (auto& molecule : molecules_)
    {
        double speed = molecule->speed().length();
        energy += molecule->mass() * speed * speed / 2;
    }

    double temperature = (molecules_.size() == 0 ? 0 : energy / molecules_.size());

    static const double aligningMassCoeff = 1e-3;

    return temperature * aligningMassCoeff;
}

double MoleculeManager::getPressure() const
{
    return 0; // TODO: Implement
}

Forcer& MoleculeManager::forcer() &
{
    if (forcerPosInBoundariesList_ < 0)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::InvalidIterator, 
            "invalid iterator when trying to access forcer",
            nullptr
        );
    }

    auto forcerIt = boundaries_.begin();
    std::advance(forcerIt, forcerPosInBoundariesList_);
    return *forcerIt;
}

void MoleculeManager::forcer(const Forcer& forcer)
{
    if (forcerPosInBoundariesList_ >= 0)
    {
        auto forcerIt = boundaries_.begin();
        std::advance(forcerIt, forcerPosInBoundariesList_); 
        boundaries_.erase(forcerIt);
    }

    boundaries_.push_back(forcer);
    forcerPosInBoundariesList_ = static_cast<int>(boundaries_.size()) - 1;
}

} // namespace Simulator
