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

    void addMolecules   (Simulator::MoleculeType moleculeType);
    void removeMolecules(Simulator::MoleculeType moleculeType);

    double getTemperature() const;
    double getPressure() const;

    std::list<std::unique_ptr<Simulator::Molecule> >& molecules() &;
};

} // namespace Simulator

#endif // CONTROLLER_CONTROLLER_HPP