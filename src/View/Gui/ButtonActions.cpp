#include "View/Gui/ButtonActions.hpp"

namespace View
{

ChangeMoleculesQuantityAction::ChangeMoleculesQuantityAction(
    Model::MoleculeManager& manager, Model::MoleculeType moleculesType, ActionType actionType
) : moleculeManager_(manager), moleculesType_(moleculesType), actionType_(actionType)
{
}

void ChangeMoleculesQuantityAction::operator()()
{
    switch (actionType_)
    {
        case ActionType::Add:
            moleculeManager_.addMolecules(moleculesType_);
            break;
        
        case ActionType::Remove:
            moleculeManager_.removeMolecules(moleculesType_);
            break;
            
        default:
            break;
    }
}

} // namespace View