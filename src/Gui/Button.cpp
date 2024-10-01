#include "Gui/Button.hpp"
#include "Gui/Action.hpp"

#include "Graphics/Mouse.hpp"

#include "Engine/Vector.hpp"

#include <iostream>

namespace Gui
{

Button::CtorParams::CtorParams(
    const Engine::Point& topLeft, unsigned int width, unsigned int height,
    bool showing, State state,
    const Graphics::Sprite& normalSprite, const Graphics::Sprite& hoverSprite,
    const Graphics::Sprite& releaseSprite, const Graphics::Sprite& pressedSprite
) : topLeft(topLeft), width(width), height(height), showing(showing), state(state),
    normalSprite(normalSprite), hoverSprite(hoverSprite), 
    releaseSprite(releaseSprite), pressedSprite(pressedSprite)
{
}

Button::Button(
    const Engine::Point& topLeft, unsigned int width, unsigned int height, bool showing, State state,
    const Graphics::Sprite& initNormalSprite, const Graphics::Sprite& initHoverSprite, 
    const Graphics::Sprite& initReleaseSprite, const Graphics::Sprite& initPressedSprite
) : topLeft_(topLeft), width_(width), height_(height), showing_(showing), state_(state),
    normalSprite_(initNormalSprite), hoveredSprite_(initHoverSprite), 
    releasedSprite_(initReleaseSprite), pressedSprite_(initPressedSprite) 
{
    switch (state_)
    {
        case State::Normal:
            sprite_ = normalSprite_;
            break;
        
        case State::Released:
            sprite_ = releasedSprite_;
            break;
        
        case State::Pressed:
            sprite_ = pressedSprite_;
            break;
        
        case State::Inactive:
            sprite_ = normalSprite_;
            break;
        
        default:
            break;
    }

    sprite_ = normalSprite_;
}

Button::Button(
    const Engine::Point& topLeft, const Graphics::Sprite& oneSpriteForAll, 
    const CtorParams& otherParams
) : Button(
        topLeft, otherParams.width, otherParams.height, otherParams.showing, otherParams.state,
        oneSpriteForAll, oneSpriteForAll, oneSpriteForAll, oneSpriteForAll
    )
{
}

bool Button::isHovered(const Graphics::RenderWindow& window) const
{
    Graphics::WindowPoint mousePos = Graphics::Mouse::getPosition(window);

    return isHovered(mousePos.x, mousePos.y);
}

bool Button::isHovered(int mousePosX, int mousePosY) const
{
    if (topLeft_.x <= mousePosX && mousePosX <= topLeft_.x + width_ &&
        topLeft_.y <= mousePosY && mousePosY <= topLeft_.y + height_)
        return true;

    return false;    
}

bool Button::update(Graphics::RenderWindow& window, const Graphics::Event& event)
{
    bool hoveredByInteractionMouse = 
        (event.type == Graphics::Event::EventType::MouseButtonReleased || 
         event.type == Graphics::Event::EventType::MouseButtonPressed) &&
        isHovered(event.mouseButton.x, event.mouseButton.y);

    if (!isHovered(window) && !hoveredByInteractionMouse)
    {
        onUnhover(window, event);
        return true;
    }

    switch (event.type)
    {
        case Graphics::Event::EventType::MouseButtonReleased:
            onRelease(window, event);
            break;
        
        case Graphics::Event::EventType::MouseButtonPressed:
            onPress(window, event);
            break;
        
        default:
            onHover(window, event);
            break;
    }

    return true;
}

void Button::draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs)
{
    Graphics::WindowVector scale = cs.getScaleInPixels(Engine::Vector{width_, height_, 0});
    Graphics::WindowPoint  pos   = cs.getPointInWindow(topLeft_);

    sprite_.scaleInPixels(scale);
    sprite_.setPosition(pos);

    renderWindow.drawSprite(sprite_);
}

void Button::onPress(Graphics::RenderWindow& window, const Graphics::Event& event)
{
    return;
}

void Button::onRelease(Graphics::RenderWindow& window, const Graphics::Event& event)
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

void Button::onHover  (Graphics::RenderWindow& window, const Graphics::Event& event)
{
    if (state_ != State::Released)
        sprite_ = hoveredSprite_;
    else
        completeActions(actions_);
}

void Button::onUnhover(Graphics::RenderWindow& window, const Graphics::Event& event)
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

int Button::addAction(std::unique_ptr<Action>&& action)
{
    actions_.push_back(std::move(action));
    return actions_.size() - 1;
}

int Button::addUndoAction(std::unique_ptr<Action>&& action)
{
    undoActions_.push_back(std::move(action));
    return undoActions_.size() - 1;
}

void completeActions(const std::vector<std::unique_ptr<Action> >& actions)
{
    for (auto& action : actions)
        (*action.get())();
}

} // namespace Gui