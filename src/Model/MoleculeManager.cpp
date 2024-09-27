#include "Model/Collider.hpp"
#include "Model/MoleculeManager.hpp"

#include "src/Model/2DVtable.hpp"

#include "Utils/Rand.hpp"

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

void handleCollisionWithBoundaries(
    ListType<std::unique_ptr<Molecule> >& molecules, ListType<Boundary> boundaries
)
{
    for (auto& molecule : molecules)
    {
        for (auto& boundary : boundaries)
        {
            if (Molecules2DVtable::checkCollision(molecule.get(), &boundary))
            {
                bounceFromBoundary(molecule.get(), &boundary);
            }
        }
    }
}

CircleMolecule generateCircleMolecule(const Point& topLeftBoundary, const Point& bottomRightBoundary)
{
    double radius   = CircleMolecule::basicRadius();
    double mass     = CircleMolecule::basicMass();
    double speedAbs = CircleMolecule::basicSpeedAbs();

    Point topLeft = topLeftBoundary;
    const double width = bottomRightBoundary.x - topLeftBoundary.x;
    const double height = bottomRightBoundary.y - topLeftBoundary.y;

    topLeft.x += Utils::Rand(0, 1) * width;
    topLeft.y += Utils::Rand(0, 1) * height;

    double vX = Utils::Rand(0, 1) * speedAbs;
    double vY = std::sqrt(speedAbs * speedAbs - vX * vX);

    Vector speed = Vector(vX, vY, 0);

    return CircleMolecule{radius, Molecule::CtorParams{topLeft, mass, speed}};
}

void generateCircleMolecules(
    ListType<std::unique_ptr<Molecule> >& molecules, const Point& topLeft, const Point& bottomRight
)
{
    static const size_t numberOfMoleculesToAdd = 10;

    for (size_t i = 0; i < numberOfMoleculesToAdd; ++i)
    {
        molecules.push_back(
            std::unique_ptr<Molecule>{new CircleMolecule{generateCircleMolecule(topLeft, bottomRight)}}
        );
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

} // namespace anon

MoleculeManager::MoleculeManager(const Point& boundaryTopLeft, const Point& boundaryBottomRight) : 
    boundaryTopLeft_(boundaryTopLeft), boundaryBottomRight_(boundaryBottomRight)
{
}

void MoleculeManager::moveMolecules()
{
    for (auto& molecule : molecules_)
    {
       molecule->move(molecule->speed()); 
    }

    handleCollisionWithBoundaries(molecules_, boundaries_);

    handleCollisionBetweenMolecules(molecules_);

    returnOutOfBoundaries(molecules_, boundaryTopLeft_, boundaryBottomRight_);
}

void MoleculeManager::addMolecules(MoleculeType moleculeType)
{
    switch (moleculeType)
    {
        case MoleculeType::Circle:
            generateCircleMolecules(molecules_, boundaryTopLeft_, boundaryBottomRight_);
            break;
        
        //TODO:

        default:
            assert(false);
            break;
    }
}

void MoleculeManager::removeMolecules(MoleculeType moleculeType)
{
    // TODO: implement
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
