#ifndef GUI_ON_RELEASE_BUTTON_HPP
#define GUI_ON_RELEASE_BUTTON_HPP

#include "Gui/Button.hpp"

namespace Gui
{

class OnReleaseButton : public Button
{
public:
    OnReleaseButton(const Button::CtorParams& params);

    OnReleaseButton(
        const Engine::Point& topLeft, const Graphics::Sprite& oneSpriteForAll, 
        const Button::CtorParams& otherParams
    );
    
private:
    virtual void onPress    (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    virtual void onRelease  (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    virtual void onHover    (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    virtual void onUnhover  (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
};

} // namespace Gui

#endif // GUI_ON_RELEASE_BUTTON_HPP