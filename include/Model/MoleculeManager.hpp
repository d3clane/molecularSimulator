#ifndef MODEL_MOLECULE_MANAGER_HPP
#define MODEL_MOLECULE_MANAGER_HPP


#include "Model/Molecule.hpp"
#include "Model/Boundary.hpp"
#include "Model/Forcer.hpp"

#include <list>
#include <memory>

namespace Simulator
{

template<typename T>
using ListType = typename std::list<T>;

template<typename T>
using ListIterator = typename ListType<T>::iterator;

class MoleculeManager
{
    ListType<std::unique_ptr<Molecule> > molecules_;
    ListType<Boundary> boundaries_;

    Point boundaryTopLeft_;
    Point boundaryBottomRight_;

    std::chrono::steady_clock::time_point prevTime_;

    int forcerPosInBoundariesList_ = -1;
    //ListIterator<Boundary> forcerIterator; // TODO: don't know how to check iterator on isValid

public:
    MoleculeManager(const Point& boundaryTopLeft, const Point& boundaryBottomRight);
    
    void moveMolecules();

    void addMolecule    (std::unique_ptr<Molecule> molecule);
    void removeMolecules(const Point& topLeft, const Point& bottomRight);

    Forcer& forcer() &;
    void forcer(const Forcer& forcer);

    double getTemperature() const;
    double getPressure()    const;

    ListType<std::unique_ptr<Molecule> >& molecules() & { return molecules_;  }
    ListType<Boundary>& boundaries() &                  { return boundaries_; }

    const ListType<std::unique_ptr<Molecule> >& molecules() const & { return molecules_;  }
    const ListType<Boundary>& boundaries() const &                  { return boundaries_; }

    Point boundaryTopLeft()     const { return boundaryTopLeft_; }
    Point boundaryBottomRight() const { return boundaryBottomRight_; }
};

} // namespace Model


#endif // MODEL_MOLECULE_MANAGER_HPP