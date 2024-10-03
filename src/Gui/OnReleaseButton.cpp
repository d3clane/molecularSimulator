#include "Gui/OnReleaseButton.hpp"
#include "Gui/Action.hpp"

#include "Graphics/Mouse.hpp"

#include "Engine/Vector.hpp"

#include <iostream>

namespace Gui
{

OnReleaseButton::OnReleaseButton(const Button::CtorParams& params) : Button(params)
{
}

OnReleaseButton::OnReleaseButton(
    const Engine::Point& topLeft, const Graphics::Sprite& oneSpriteForAll, 
    const Button::CtorParams& otherParams
) : Button(
        topLeft, otherParams.width, otherParams.height, otherParams.showing, otherParams.state, 
        oneSpriteForAll, oneSpriteForAll, oneSpriteForAll, oneSpriteForAll
    )
{
}

void OnReleaseButton::onPress(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    return;
}

void OnReleaseButton::onRelease(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
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

void OnReleaseButton::onHover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    if (state_ != State::Released)
        sprite_ = hoveredSprite_;
    else
        completeActions(actions_);
}

void OnReleaseButton::onUnhover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    switch (state_)
    {
        case State::Released:
            completeActions(actions_);
            break;

        case State::Normal:
            break;

        default: // unreachable
            assert(false);
            break;
    }
}

} // namespace Gui