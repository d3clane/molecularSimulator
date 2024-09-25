#include "Engine/Collider.hpp"

#include "Scene/MoleculeManager.hpp"

#include <iostream>

namespace Scene
{

void onCircleMoleculesCollision(
    ListIterator<CircleMolecule>& aIt, ListIterator<CircleMolecule>& bIt
)
{
    CircleMolecule& a = *aIt;
    CircleMolecule& b = *bIt;

    Vector aSpeed = a.speed();
    Vector bSpeed = b.speed();

    unsigned int aMass = a.mass();
    unsigned int bMass = b.mass();

    Vector relativeASpeedBefore = aSpeed - bSpeed;

    Vector centerDirection              = Vector(b.topLeft(), a.topLeft());
    Vector perpendicularDirection       = centerDirection.getPerpendicular();
    Vector centerDirectionSpeed         = relativeASpeedBefore.projectOnto(centerDirection);
    Vector perpendicularDirectionSpeed  = relativeASpeedBefore.projectOnto(perpendicularDirection);

    double v_x = centerDirectionSpeed.length();
    double v_y = perpendicularDirectionSpeed.length();

    double relativeBSpeedAfterLen = 2 * aMass * bMass * (v_x + v_y) / (aMass * bMass + bMass * bMass);

    Vector relativeBSpeedAfter = centerDirection.getNormalizedVector() * relativeBSpeedAfterLen;
    Vector relativeASpeedAfter = relativeASpeedBefore - (bMass / aMass) * relativeBSpeedAfter;

    Vector aSpeedAfter = bSpeed + relativeASpeedAfter;
    Vector bSpeedAfter = bSpeed + relativeBSpeedAfter;

    a.speed(aSpeedAfter);
    b.speed(bSpeedAfter);
}

void onRectangleMoleculesCollision(ListIterator<RectangleMolecule>& aIt, ListIterator<RectangleMolecule>& bIt)
{

}

template<typename T1, typename F>
void handleMoleculesCollision(ListType<T1>& molecules, F onCollision)
{
    for (auto firstMoleculeIt = molecules.begin(), itEnd = molecules.end(); 
         firstMoleculeIt != itEnd; ++firstMoleculeIt)
    {
        auto secondMoleculeIt = firstMoleculeIt;
        ++secondMoleculeIt;
        
        for (; secondMoleculeIt != itEnd; ++secondMoleculeIt)
        {
            if (checkCollision(firstMoleculeIt->collider(), secondMoleculeIt->collider()))
                onCollision(firstMoleculeIt, secondMoleculeIt);
        }
    }
}

template<typename T>
void onCollisionWithBoundary(T& molecule, const Boundary& boundary)
{
    Vector speed = molecule.speed();
    Vector newSpeed = -speed.reflectRelatively(boundary.perpendicular());

    molecule.speed(newSpeed);
}

template<typename T, typename F>
void handleCollisionWithBoundary(ListType<T>& molecules, ListType<Boundary> boundaries, F onCollision)
{
    for (auto& molecule : molecules)
    {
        for (auto& boundary : boundaries)
        {
            if (checkCollision(molecule.collider(), boundary.collider()))
                onCollision(molecule, boundary);
        }
    }
}

void onCircleAndRectangleMoleculesCollision(
    CircleMolecule& circleMolecule, RectangleMolecule& rectangleMolecule
)
{

}

void handleCollisionCircleWithRectangle(
    ListType<CircleMolecule>& circleMolecules, ListType<RectangleMolecule>& rectangleMolecules
)
{
    for (auto& circleMolecule : circleMolecules)
    {
        for (auto& rectangleMolecule : rectangleMolecules)
        {
            if (checkCollision(circleMolecule.collider(), rectangleMolecule.collider()))
                onCircleAndRectangleMoleculesCollision(circleMolecule, rectangleMolecule);
        }
    }
}

void MoleculeManager::moveMolecules()
{
    for (auto& molecule : circleMolecules_)
    {
        molecule.move(molecule.speed());
        molecule.collider() = Engine::CircleCollider(molecule.topLeft(), molecule.radius());
    }
    
    handleMoleculesCollision(
        circleMolecules_, onCircleMoleculesCollision
    );
    handleCollisionWithBoundary(
        circleMolecules_, boundaries_, onCollisionWithBoundary<CircleMolecule>
    );
}

ListType<CircleMolecule>&          MoleculeManager::circleMolecules   ()       { return circleMolecules_; }
const ListType<CircleMolecule>&    MoleculeManager::circleMolecules   () const { return circleMolecules_; }
ListType<RectangleMolecule>&       MoleculeManager::rectangleMolecules()       { return rectangleMolecules_; }
const ListType<RectangleMolecule>& MoleculeManager::rectangleMolecules() const { return rectangleMolecules_; }
ListType<Boundary>&                MoleculeManager::boundaries        ()       { return boundaries_; }
const ListType<Boundary>&          MoleculeManager::boundaries        () const { return boundaries_; }

Boundary::Boundary(
    const Point& topLeft, const unsigned int width, const unsigned int height,
    const Vector& perpendicular
) : Engine::Transformable(topLeft), width_(width), height_(height), perpendicular_(perpendicular),
    collider_(Engine::RectangleCollider(topLeft, width, height))
{
}

Engine::RectangleCollider& Boundary::collider() & { return collider_; }
const Vector& Boundary::perpendicular() const &   { return perpendicular_; }

}