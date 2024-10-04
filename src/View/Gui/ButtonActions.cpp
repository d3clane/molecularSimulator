#include "View/Gui/ButtonActions.hpp"

namespace Simulator
{

ChangeMoleculesQuantityAction::ChangeMoleculesQuantityAction(
    Simulator::Controller& controller, ActionType actionType
) : controller_(controller), actionType_(actionType)
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

MoveForcerAction::MoveForcerAction(
    Simulator::Controller& controller, ActionType actionType
) : controller_(controller), actionType_(actionType) 
{
}

void MoveForcerAction::operator()()
{
    std::chrono::milliseconds deltaTime{10}; // TODO: 
    switch (actionType_)
    {
        case ActionType::MoveUp:
            controller_.moveForcerUp(deltaTime);
            break;
        case ActionType::MoveDown:
            controller_.moveForcerDown(deltaTime);
            break;
        
        default:
            break;
    }
}

} // namespace Simulator