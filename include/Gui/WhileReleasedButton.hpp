#ifndef GUI_WHILE_RELEASED_BUTTON_HPP
#define GUI_WHILE_RELEASED_BUTTON_HPP

#include "Gui/Button.hpp"

namespace Gui
{

class WhileReleasedButton : public Button
{
public:
    WhileReleasedButton(const CtorParams& params);

private:
    virtual void onPress    (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    virtual void onRelease  (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    virtual void onHover    (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    virtual void onUnhover  (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
};

} // namespace Gui

#endif // GUI_WHILE_RELEASED_BUTTON_HPP