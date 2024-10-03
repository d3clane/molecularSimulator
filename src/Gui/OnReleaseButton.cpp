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
    completeActions(actions_);
}

void OnReleaseButton::onHover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    return;
}

void OnReleaseButton::onUnhover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    return;
}

} // namespace Gui