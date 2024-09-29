#include "Controller/Controller.hpp"
#include "Model/Molecule.hpp"
#include "Utils/Rand.hpp"

namespace Simulator
{

namespace 
{

using Engine::Point;
using Engine::Vector;

Simulator::CircleMolecule generateCircleMolecule(
    const Point& topLeftBoundary, const Point& bottomRightBoundary
)
{
    double radius   = Simulator::CircleMolecule::basicRadius();
    double mass     = Simulator::CircleMolecule::basicMass();
    double speedAbs = Simulator::CircleMolecule::basicSpeedAbs();

    Point topLeft = topLeftBoundary;
    const double width = bottomRightBoundary.x - topLeftBoundary.x;
    const double height = bottomRightBoundary.y - topLeftBoundary.y;

    topLeft.x += Utils::Rand(0, 1) * width;
    topLeft.y += Utils::Rand(0, 1) * height;

    double vX = Utils::Rand(0, 1) * speedAbs;
    double vY = std::sqrt(speedAbs * speedAbs - vX * vX);

    Vector speed = Vector(vX, vY, 0);

    return Simulator::CircleMolecule{radius, Simulator::Molecule::CtorParams{topLeft, mass, speed}};
}

} // namespace anon

Controller::Controller(Simulator::MoleculeManager& manager) : manager_(manager) {}
void Controller::addMolecules(Simulator::MoleculeType moleculeType) // TODO: implement based on moleculeType
{
    static const size_t numberOfMoleculesToAdd = 10;

    for (size_t i = 0; i < numberOfMoleculesToAdd; ++i)
    {
        Simulator::CircleMolecule circleMolecule = generateCircleMolecule(
            manager_.boundaryTopLeft(), manager_.boundaryBottomRight()
        );

        manager_.addMolecule(std::unique_ptr<Simulator::Molecule>{new Simulator::CircleMolecule{circleMolecule}});
    }
}
void Controller::removeMolecules(Simulator::MoleculeType moleculeType)
{
    manager_.removeMolecules(manager_.boundaryTopLeft(), manager_.boundaryBottomRight());
}

std::list<std::unique_ptr<Simulator::Molecule> >& Controller::molecules() &
{
    return manager_.molecules();
}

} // namespace Controller