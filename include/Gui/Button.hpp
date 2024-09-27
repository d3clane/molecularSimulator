#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Graphics/GraphicsWindow.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Events.hpp"

#include "Gui/Window.hpp"

#include "Engine/CoordsSystem.hpp"

#include <memory>

namespace Gui
{

class Action;

class Button : public Window
{
public:
    enum class State
    {
        Normal,
        Released,
        Pressed,
        Inactive,
    };

    struct CtorParams
    {
        Engine::Point topLeft;
        unsigned int width;
        unsigned int height;
        bool showing;
        State state;
        Graphics::Sprite normalSprite;
        Graphics::Sprite hoverSprite;
        Graphics::Sprite releaseSprite;
        Graphics::Sprite pressedSprite;

        CtorParams(
            const Engine::Point& topLeft, unsigned int width, unsigned int height, 
            bool showing, State state, 
            const Graphics::Sprite& normalSprite, const Graphics::Sprite& hoverSprite,
            const Graphics::Sprite& releaseSprite, const Graphics::Sprite& pressedSprite
        );
    };

    Button(
        const Engine::Point& topLeft, unsigned int width, unsigned int height, bool showing, State state,
        const Graphics::Sprite& initNormalSprite, const Graphics::Sprite& initHoverSprite, 
        const Graphics::Sprite& initReleaseSprite, const Graphics::Sprite& initPressedSprite
    );

    Button(
        const Engine::Point& topLeft, const Graphics::Sprite& oneSpriteForAll, 
        const CtorParams& otherParams
    );
    
    // functions
    bool isHovered(const Graphics::RenderWindow& window) const;
    bool isHovered(int mousePosX, int mousePosY)   const;
    
    bool showing() const                   { return showing_; }
    void showing(bool newShowingCondition) { showing_ = newShowingCondition; }

    State state() const        { return state_; }
    void state(State newState) { state_ = newState; }

    int  addAction    (std::unique_ptr<Action>&& action);
    int  addUndoAction(std::unique_ptr<Action>&& action);

    virtual bool update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event  ) override;
    virtual void draw  (Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs) override;

    operator Graphics::Sprite();

private:
    virtual void onPress    (Graphics::RenderWindow& window, const Graphics::Event& event);
    virtual void onRelease  (Graphics::RenderWindow& window, const Graphics::Event& event);
    virtual void onHover    (Graphics::RenderWindow& window, const Graphics::Event& event);
    virtual void onUnhover  (Graphics::RenderWindow& window, const Graphics::Event& event);

protected:
    Engine::Point topLeft_;
    unsigned int width_, height_;

    Graphics::Sprite sprite_;
    bool showing_;
    State state_;

    Graphics::Sprite normalSprite_;
    Graphics::Sprite hoveredSprite_;
    Graphics::Sprite releasedSprite_;
    Graphics::Sprite pressedSprite_;

    std::vector<std::unique_ptr<Action> > actions_;
    std::vector<std::unique_ptr<Action> > undoActions_;
};

void completeActions(const std::vector<std::unique_ptr<Action> >& actions);

} // namespace Gui

#endif