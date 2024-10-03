#include "Gui/WhileReleasedButton.hpp"
#include "Gui/Action.hpp"

namespace Gui
{

WhileReleasedButton::WhileReleasedButton(const CtorParams& params) : Button(params)
{
}

void WhileReleasedButton::onPress(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    return;
}

void WhileReleasedButton::onRelease(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    if (state_ == State::Released)
    {
        state_  = State::Normal;
        sprite_ = normalSprite_;

        completeActions(undoActions_);
    }
    else
    {
        sprite_ = releasedSprite_;

        completeActions(actions_);
    }
}

void WhileReleasedButton::onHover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    if (state_ != State::Released)
        sprite_ = hoveredSprite_;
    else
        completeActions(actions_);
}

void WhileReleasedButton::onUnhover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    if (state_ != State::Released)
        sprite_ = normalSprite_;
    else
        completeActions(undoActions_);
}

} // namespace Gui