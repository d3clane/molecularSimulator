#include "Gui/Button.hpp"
#include "Gui/Action.hpp"

#include "Graphics/Mouse.hpp"

namespace Gui
{

namespace 
{

void configureSprite(
    Graphics::Sprite& sprite, 
    unsigned int width, unsigned int height, const Graphics::WindowPoint& topLeft
)
{
    sprite.setPosition(topLeft);
    sprite.scaleInPixels({width, height});
}

} // namespace anonymous

Button::CtorParams::CtorParams(
    const Graphics::WindowPoint& topLeft, unsigned int width, unsigned int height,
    bool showing, State state,
    const Graphics::Sprite& normalSprite, const Graphics::Sprite& hoverSprite,
    const Graphics::Sprite& releaseSprite, const Graphics::Sprite& pressedSprite
) : topLeft(topLeft), width(width), height(height), showing(showing), state(state),
    normalSprite(normalSprite), hoverSprite(hoverSprite), 
    releaseSprite(releaseSprite), pressedSprite(pressedSprite)
{
}

Button::Button(
    const Graphics::WindowPoint& topLeft, unsigned int width, unsigned int height, bool showing, State state,
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

    configureSprite(normalSprite_,   width_, height_, topLeft_);
    configureSprite(releasedSprite_, width_, height_, topLeft_);
    configureSprite(hoveredSprite_,  width_, height_, topLeft_);
    configureSprite(pressedSprite_,  width_, height_, topLeft_);

    sprite_ = normalSprite_;
}

Button::Button(
    const Graphics::WindowPoint& topLeft, const Graphics::Sprite& oneSpriteForAll, 
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

void Button::update(Graphics::RenderWindow& window, const Graphics::Event& event)
{
    bool hoveredByInteractionMouse = 
        (event.type == Graphics::Event::EventType::MouseButtonReleased || 
         event.type == Graphics::Event::EventType::MouseButtonPressed) &&
        isHovered(event.mouseButton.x, event.mouseButton.y);

    if (!isHovered(window) && !hoveredByInteractionMouse)
    {
        onUnhover(window, event);
        return;
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


void Button::draw(Graphics::RenderWindow& renderWindow)
{
    sprite_.scaleInPixels({width_, height_});
    sprite_.setPosition(topLeft_);

    renderWindow.drawSprite(sprite_);
}

Button::operator Graphics::Sprite()
{
    sprite_.scaleInPixels({width_, height_});
    sprite_.setPosition(topLeft_);
    
    return sprite_;
}

int Button::addAction(Action* action)
{
    actions_.push_back(action);
    return actions_.size() - 1;
}

int Button::addUndoAction(Action* action)
{
    undoActions_.push_back(action);
    return undoActions_.size() - 1;
}

void Button::deleteAction(Action* action)
{
    std::remove(actions_.begin(), actions_.end(), action);
}

void Button::deleteUndoAction(Action* action)
{
    std::remove(undoActions_.begin(), undoActions_.end(), action);
}

void Button::deleteAction(int pos)
{
    actions_.erase(actions_.begin() + pos);
}

void Button::deleteUndoAction(int pos)
{
    undoActions_.erase(undoActions_.begin() + pos);
}

void completeActions(const std::vector< Action* >& actions)
{
    for (Action* action : actions)
        (*action)();
}

} // namespace Gui