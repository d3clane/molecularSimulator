#include "View/Gui/ButtonActions.hpp"

namespace Simulator
{

ChangeMoleculesQuantityAction::ChangeMoleculesQuantityAction(
    Simulator::Controller& controller, Simulator::MoleculeType moleculesType, ActionType actionType
) : controller_(controller), moleculesType_(moleculesType), actionType_(actionType)
{
}

void ChangeMoleculesQuantityAction::operator()()
{
    switch (actionType_)
    {
        case ActionType::Add:
            controller_.addMolecules(moleculesType_);
            break;
        
        case ActionType::Remove:
            controller_.removeMolecules(moleculesType_);
            break;
            
        default:
            break;
    }
}

} // namespace View