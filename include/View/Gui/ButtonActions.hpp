#ifndef VIEW_GUI_BUTTON_ACTIONS_HPP
#define VIEW_GUI_BUTTON_ACTIONS_HPP

#include "Model/MoleculeManager.hpp"
#include "Model/Molecule.hpp"
#include "Gui/Action.hpp"

namespace View
{

class ChangeMoleculesQuantityAction : public Gui::Action
{
    Model::MoleculeManager& moleculeManager_;
    Model::MoleculeType  moleculesType_;
    
public:
    enum class ActionType
    {
        Add,
        Remove
    };

    ChangeMoleculesQuantityAction(
        Model::MoleculeManager& manager, Model::MoleculeType moleculesType, ActionType actionType
    );

    virtual void operator()() override;

private:
    ActionType actionType_;
};

} // namespace View

#endif // VIEW_GUI_BUTTON_ACTIONS_HPP