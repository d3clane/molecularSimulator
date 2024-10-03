#include "Gui/Button.hpp"
#include "Gui/Action.hpp"

#include "Graphics/Mouse.hpp"

#include "Engine/Vector.hpp"

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

Button::Button(const Button::CtorParams& ctorParams) :
    Button(
        ctorParams.topLeft, ctorParams.width, ctorParams.height, ctorParams.showing, ctorParams.state,
        ctorParams.normalSprite, ctorParams.hoverSprite, ctorParams.releaseSprite, ctorParams.pressedSprite
    )
{
}

bool Button::isHovered(const Graphics::RenderWindow& renderWindow) const
{
    Graphics::WindowPoint mousePos = Graphics::Mouse::getPosition(renderWindow);

    return isHovered(mousePos.x, mousePos.y);
}

bool Button::isHovered(int mousePosX, int mousePosY) const
{
    if (topLeft_.x <= mousePosX && mousePosX <= topLeft_.x + width_ &&
        topLeft_.y <= mousePosY && mousePosY <= topLeft_.y + height_)
        return true;

    return false;    
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

Button::operator Graphics::Sprite() const
{
    return sprite_;
}

void Button::draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs)
{
    Graphics::WindowVector scale = cs.getScaleInPixels(Engine::Vector{width_, height_, 0});
    Graphics::WindowPoint  pos   = cs.getPointInWindow(topLeft_);

    sprite_.scaleInPixels(scale);
    sprite_.setPosition(pos);

    renderWindow.drawSprite(sprite_);
}

bool Button::update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    bool hoveredByInteractionMouse = 
        (event.type == Graphics::Event::EventType::MouseButtonReleased || 
         event.type == Graphics::Event::EventType::MouseButtonPressed) &&
        isHovered(event.mouseButton.x, event.mouseButton.y);

    if (!isHovered(renderWindow) && !hoveredByInteractionMouse)
    {
        onUnhover(renderWindow, event);
        return true;
    }

    switch (event.type)
    {
        case Graphics::Event::EventType::MouseButtonReleased:
            onRelease(renderWindow, event);
            break;
        
        case Graphics::Event::EventType::MouseButtonPressed:
            onPress(renderWindow, event);
            break;
        
        default:
            onHover(renderWindow, event);
            break;
    }

    return true;
}

void completeActions(const std::vector<std::unique_ptr<Action> >& actions)
{
    for (auto& action : actions)
        (*action.get())();
}

} // namespace Gui
