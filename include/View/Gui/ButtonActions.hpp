#ifndef VIEW_GUI_BUTTON_ACTIONS_HPP
#define VIEW_GUI_BUTTON_ACTIONS_HPP

#include "Controller/Controller.hpp"
#include "Model/Molecule.hpp"
#include "Gui/Action.hpp"

namespace Simulator
{

class ChangeMoleculesQuantityAction : public Gui::Action
{
    Simulator::Controller& controller_;
    Simulator::MoleculeType  moleculesType_;
    
public:
    enum class ActionType
    {
        Add,
        Remove
    };

    ChangeMoleculesQuantityAction(
        Simulator::Controller& controller, Simulator::MoleculeType moleculesType, ActionType actionType
    );

    virtual void operator()() override;

private:
    ActionType actionType_;
};

} // namespace Simulator

#endif // VIEW_GUI_BUTTON_ACTIONS_HPP