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

    double vX = Utils::Rand(-1, 1) * speedAbs;
    double vY = std::sqrt(speedAbs * speedAbs - vX * vX) * Utils::RandDirection();

    Vector speed = Vector(vX, vY, 0);

    return Simulator::CircleMolecule{radius, Simulator::Molecule::CtorParams{topLeft, mass, speed}};
}

} // namespace anon

Controller::Controller(Simulator::MoleculeManager& manager) : moleculeManager_(manager) {}
void Controller::addCircleMolecules()
{
    static const size_t numberOfMoleculesToAdd = 10;

    for (size_t i = 0; i < numberOfMoleculesToAdd; ++i)
    {
        Simulator::CircleMolecule circleMolecule = generateCircleMolecule(
            moleculeManager_.boundaryTopLeft(), moleculeManager_.boundaryBottomRight()
        );

        moleculeManager_.addMolecule(std::unique_ptr<Simulator::Molecule>{new Simulator::CircleMolecule{circleMolecule}});
    }
}
void Controller::removeMolecules()
{
    moleculeManager_.removeMolecules(moleculeManager_.boundaryTopLeft(), moleculeManager_.boundaryBottomRight());
}

std::list<std::unique_ptr<Simulator::Molecule> >& Controller::molecules() &
{
    return moleculeManager_.molecules();
}

double Controller::getTemperature() const
{
    return moleculeManager_.getTemperature();
}

double Controller::getPressure() const
{
    return moleculeManager_.getPressure();
}

void Controller::addBoundary(const Boundary& boundary)
{
    auto& boundaries = moleculeManager_.boundaries();

    boundaries.push_back(boundary);
}

void Controller::moveForcerUp(std::chrono::milliseconds deltaTime)
{
    static Engine::Vector up{0, -0.01, 0};

    moleculeManager_.forcer().move(up * deltaTime.count());
}

void Controller::moveForcerDown(std::chrono::milliseconds deltaTime)
{
    static Engine::Vector down{0, 0.01, 0};

    moleculeManager_.forcer().move(down * deltaTime.count());
}

} // namespace Simulator