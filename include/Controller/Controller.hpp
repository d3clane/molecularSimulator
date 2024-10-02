#ifndef CONTROLLER_CONTROLLER_HPP
#define CONTROLLER_CONTROLLER_HPP

#include "Model/MoleculeManager.hpp"
#include "Graphics/Events.hpp"

namespace Simulator
{

class Controller final
{
    Simulator::MoleculeManager& moleculeManager_;

public:
    Controller(Simulator::MoleculeManager& manager);

    void addCircleMolecules();
    void removeMolecules();

    double getTemperature() const;
    double getPressure() const;

    void addBoundary(const Boundary& boundary);

    void moveForcerUp  (std::chrono::milliseconds deltaTime);
    void moveForcerDown(std::chrono::milliseconds deltaTime);

    // view: controller.updateModel()
    // btn.action() -> controller.moveForcerUp() -> forcer moves
    // controller.updateForcer()
    // controller
    // model - doesn't interact. Interaction only on press

    std::list<std::unique_ptr<Simulator::Molecule> >& molecules() &;
};

} // namespace Simulator

#endif // CONTROLLER_CONTROLLER_HPP