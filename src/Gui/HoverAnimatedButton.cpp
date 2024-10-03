#include "Graphics/PixelsArray.hpp"
#include "Gui/HoverAnimatedButton.hpp"

#include "Gui/Action.hpp"

#include <iostream>

namespace Gui
{

namespace 
{

// TODO: COPY_PASTE from button.cpp think about it
void configureSprite(
    Graphics::Sprite& sprite, const Engine::Point& topLeft, unsigned int width, unsigned int height,
    const Engine::CoordsSystem& coordsSystem
)
{
    sprite.setPosition  (coordsSystem.getPointInWindow(topLeft));
    sprite.scaleInPixels(coordsSystem.getScaleInPixels(Engine::Vector{width, height, 0}));
}

} // namespace anon

HoverAnimatedButton::HoverAnimatedButton(
    const std::chrono::milliseconds& animationDuration, const Button::CtorParams& ctorParams
) : WhileReleasedButton(ctorParams), animation_(&normalSprite_, &hoveredSprite_, animationDuration),
    animationType_(AnimationType::Unhovering)
{
}

void HoverAnimatedButton::onRelease(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    if (state_ == State::Released)
    {
        state_  = State::Normal;
        sprite_ = normalSprite_;

        completeActions(undoActions_);
    }
    else
    {
        state_ = State::Released;
        sprite_ = releasedSprite_;

        completeActions(actions_);
    }
}

void HoverAnimatedButton::onHover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    if (state_ != State::Released)
        sprite_ = hoveredSprite_;
    else
        completeActions(actions_);

    if (animationType_ == AnimationType::Unhovering)
    {
        animationType_ = AnimationType::Hovering;
        animation_.swapAnimationDirection();
    }
}

void HoverAnimatedButton::onUnhover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    if (state_ != State::Released)
        sprite_ = normalSprite_;
    else
        completeActions(undoActions_);

    if (animationType_ == AnimationType::Hovering)
    {
        animationType_ = AnimationType::Unhovering;
        animation_.swapAnimationDirection();
    }
}

void HoverAnimatedButton::onPress(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    return;
}

void HoverAnimatedButton::draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs)
{
    configureSprite(normalSprite_  , topLeft_, width_, height_, cs);
    configureSprite(hoveredSprite_ , topLeft_, width_, height_, cs);
    configureSprite(releasedSprite_, topLeft_, width_, height_, cs);
    configureSprite(pressedSprite_ , topLeft_, width_, height_, cs);
    configureSprite(sprite_        , topLeft_, width_, height_, cs);

    switch (state_)
    {
        case State::Released:
            sprite_.draw(renderWindow);
            break;
        
        case State::Pressed:
        case State::Normal:
        default:
            animation_.draw(renderWindow, cs);
            break;
    }
}

} // namespace Gui