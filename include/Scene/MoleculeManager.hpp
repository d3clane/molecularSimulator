#ifndef SCENE_MOLECULE_MANAGER_HPP
#define SCENE_MOLECULE_MANAGER_HPP

#include <list>

#include "Scene/Molecule.hpp"

namespace Scene
{

class Boundary : public Engine::Transformable
{
    unsigned int width_;
    unsigned int height_;

    Engine::RectangleCollider collider_;

    Vector perpendicular_;

public:
    Boundary(
        const Point& topLeft, const unsigned int width, const unsigned int height,
        const Vector& perpendicular
    );

    Engine::RectangleCollider& collider() &;
    const Vector& perpendicular() const &;
};

template<typename T>
using ListType = typename std::list<T>;

template<typename T>
using ListIterator = typename ListType<T>::iterator;

class MoleculeManager
{
    ListType<CircleMolecule> circleMolecules_;
    ListType<RectangleMolecule> rectangleMolecules_;
    ListType<Boundary> boundaries_;

public:
    void moveMolecules();

    ListType<CircleMolecule>&          circleMolecules   ();
    const ListType<CircleMolecule>&    circleMolecules   () const;
    ListType<RectangleMolecule>&       rectangleMolecules();
    const ListType<RectangleMolecule>& rectangleMolecules() const;
    ListType<Boundary>&                boundaries        ();
    const ListType<Boundary>&          boundaries        () const;

private:
#if 0
    void onCircleMoleculesCollision(ListIterator<CircleMolecule>& aIt, ListIterator<CircleMolecule>& bIt);
    void onRectangleMoleculesCollision(
        ListIterator<RectangleMolecule>& aIt, ListIterator<RectangleMolecule>& bIt
    );
#endif
};

} // namespace Scene


#endif // SCENE_MOLECULE_MANAGER_HPP