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
            controller_.addCircleMolecules();
            break;
        
        case ActionType::Remove:
            controller_.removeMolecules();
            break;
            
        default:
            break;
    }
}

} // namespace Simulator