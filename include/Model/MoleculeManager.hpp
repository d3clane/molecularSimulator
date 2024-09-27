#ifndef MODEL_MOLECULE_MANAGER_HPP
#define MODEL_MOLECULE_MANAGER_HPP


#include "Model/Molecule.hpp"

#include <list>
#include <memory>

namespace Model
{

class Boundary : public Engine::Transformable
{
    double width_;
    double height_;

    std::unique_ptr<RectangleCollider> collider_;

    Vector perpendicular_;

public:
    Boundary(
        const Point& topLeft, const double width, const double height,
        const Vector& perpendicular
    );

    Boundary(const Boundary& other);
    
    const RectangleCollider* collider() const &;
    const Vector& perpendicular() const &;
};

template<typename T>
using ListType = typename std::list<T>;

template<typename T>
using ListIterator = typename ListType<T>::iterator;

class MoleculeManager
{
    ListType<std::unique_ptr<Molecule> > molecules_;
    ListType<Boundary> boundaries_;

public:
    void moveMolecules();

    ListType<std::unique_ptr<Molecule> >& molecules() & { return molecules_;  }
    ListType<Boundary>& boundaries() &                  { return boundaries_; }

    const ListType<std::unique_ptr<Molecule> >& molecules() const & { return molecules_;  }
    const ListType<Boundary>& boundaries() const &                  { return boundaries_; }
};

} // namespace Model


#endif // MODEL_MOLECULE_MANAGER_HPP