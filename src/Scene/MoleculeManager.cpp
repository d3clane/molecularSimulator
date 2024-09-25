#include "Engine/Collider.hpp"

#include "Scene/MoleculeManager.hpp"

#include <iostream>

namespace Scene
{

void onCircleMoleculesCollision(
    ListIterator<CircleMolecule>& aIt, ListIterator<CircleMolecule>& bIt,
    ListType<CircleMolecule>& circleMolecules, ListType<RectangleMolecule>& rectangleMolecules
)
{
    CircleMolecule& a = *aIt;
    CircleMolecule& b = *bIt;

    Vector aSpeed = a.speed();
    Vector bSpeed = b.speed();

    unsigned int aMass = a.mass();
    unsigned int bMass = b.mass();

    Vector relativeASpeedBefore = aSpeed - bSpeed;

    Vector centerDirection      = Vector(a.topLeft(), b.topLeft());
    Vector centerDirectionSpeed = relativeASpeedBefore.projectOnto(centerDirection);

    double v_x = centerDirectionSpeed.length();

    double relativeBSpeedAfterLen = 2 * aMass * bMass * v_x / (aMass * bMass + bMass * bMass);

    Vector relativeBSpeedAfter = centerDirection.getNormalizedVector() * relativeBSpeedAfterLen;
    Vector relativeASpeedAfter = relativeASpeedBefore - (bMass / aMass) * relativeBSpeedAfter;

    Vector aSpeedAfter = bSpeed + relativeASpeedAfter;
    Vector bSpeedAfter = bSpeed + relativeBSpeedAfter;

    //a.move(-3 * a.speed());
    //b.move(-3 * b.speed());

    a.speed(aSpeedAfter);
    b.speed(bSpeedAfter);

    //a.collider() = Engine::CircleCollider(a.topLeft(), a.radius());
    //b.collider() = Engine::CircleCollider(b.topLeft(), b.radius());
}

void onRectangleMoleculesCollision(
    ListIterator<RectangleMolecule>& aIt, ListIterator<RectangleMolecule>& bIt,
    ListType<CircleMolecule>& circleMolecules, ListType<RectangleMolecule>& rectangleMolecules
)
{
    RectangleMolecule& a = *aIt;
    RectangleMolecule& b = *bIt;

    Vector aSpeed = a.speed();
    Vector bSpeed = b.speed();

    unsigned int aMass = a.mass();
    unsigned int bMass = b.mass();

    RectangleMolecule tmp = a;
    tmp.mass(aMass + bMass);
    tmp.speed((aMass * aSpeed + bMass * bSpeed) / (aMass + bMass));

    // chem reaction
    rectangleMolecules.push_back(tmp);

    aIt = rectangleMolecules.erase(aIt);
    bIt = rectangleMolecules.erase(bIt);
}

template<typename T1, typename F>
void handleMoleculesCollision(
    ListType<T1>& molecules, 
    ListType<CircleMolecule>& circleMolecules, ListType<RectangleMolecule>& rectangleMolecules,
    F onCollision
)
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
        molecule.move(molecule.speed());

    for (auto& molecule : rectangleMolecules_)
        molecule.move(molecule.speed());
        
    handleMoleculesCollision(
        circleMolecules_, onCircleMoleculesCollision
    );
    handleMoleculesCollision(
        rectangleMolecules_, onRectangleMoleculesCollision
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
    const Point& topLeft, const double width, const double height,
    const Vector& perpendicular
) : Engine::Transformable(topLeft), width_(width), height_(height), perpendicular_(perpendicular),
    collider_(Engine::RectangleCollider(&transformableTopLeft_, &width_, &height_))
{
}

Boundary::Boundary(const Boundary& other) : 
    Boundary(other.transformableTopLeft_, other.width_, other.height_, other.perpendicular_)
{
}

Engine::RectangleCollider& Boundary::collider() & { return collider_; }
const Vector& Boundary::perpendicular() const &   { return perpendicular_; }

}