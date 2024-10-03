#ifndef AQUA_BUTTON_HPP
#define AQUA_BUTTON_HPP

#include <chrono>

#include "Graphics/Sprite.hpp"
#include "Gui/WhileReleasedButton.hpp"
#include "Gui/Animation.hpp"

namespace Gui
{

class HoverAnimatedButton : public WhileReleasedButton
{
    enum class AnimationType
    {
        Unhovering,
        Hovering,
    };

    Animation animation_;
    AnimationType animationType_;

public:
    HoverAnimatedButton(const std::chrono::milliseconds& animationDuration, const Button::CtorParams& params);

    //HoverAnimatedButton(const Engine::Point& topLeft, const Button::CtorParams& otherParams);

    void onRelease(Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    void onPress  (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    void onHover  (Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
    void onUnhover(Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;

    void draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs) override;
};

} // namespace Gui

#endif