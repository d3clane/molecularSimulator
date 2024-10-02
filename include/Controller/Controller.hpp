#ifndef CONTROLLER_CONTROLLER_HPP
#define CONTROLLER_CONTROLLER_HPP

#include "Model/MoleculeManager.hpp"
#include "Graphics/Events.hpp"

namespace Simulator
{

class Controller final
{
    Simulator::MoleculeManager& manager_;
public:

    Controller(Simulator::MoleculeManager& manager);

    void addCircleMolecules();
    void removeMolecules();

    double getTemperature() const;
    double getPressure() const;

    void addBoundary(const Boundary& boundary);

    std::list<std::unique_ptr<Simulator::Molecule> >& molecules() &;
};

} // namespace Simulator

#endif // CONTROLLER_CONTROLLER_HPP