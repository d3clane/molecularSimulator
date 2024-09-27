#ifndef AQUA_BUTTON_HPP
#define AQUA_BUTTON_HPP

#include <chrono>

#include "Graphics/Sprite.hpp"
#include "Gui/Button.hpp"

namespace Gui
{

class HoverAnimatedButton : public Button
{
    enum class AnimationType
    {
        None,
        Hovering,
        Unhovering,
    };

    struct PrevSpriteWrap
    {
    private:
        Graphics::Sprite prevSprite_;

    public:
        void setPrevSprite(const Graphics::Sprite& sprite);
        Graphics::Sprite& getPrevSpriteLink();
        
        explicit operator Graphics::Sprite&();
    };

    PrevSpriteWrap prevSprite_;

    std::chrono::steady_clock::time_point animationBeginTime_; // time hovered or unhovered
    uint8_t spriteBeginAlpha_;

    std::chrono::milliseconds animationDuration_;              // time on interaction
    AnimationType animationType_;

public:
    struct CtorParams : public Button::CtorParams
    {
        std::chrono::milliseconds animationDuration;

        CtorParams(const std::chrono::milliseconds& animationDuration, const Button::CtorParams& otherParams);
    };

    HoverAnimatedButton(
        const Graphics::WindowPoint& topLeft, unsigned int width, unsigned int height, bool showing,
        const Graphics::Sprite& normalSprite  , const Graphics::Sprite& hoveredSprite,
        const Graphics::Sprite& releasedSprite, const Graphics::Sprite& pressedSprite,
        std::chrono::milliseconds animationDuration
    );

    HoverAnimatedButton(const Graphics::WindowPoint& topLeft, const CtorParams& otherParams);

    virtual ~HoverAnimatedButton() = default;

    virtual void onRelease  (Graphics::RenderWindow& window, const Graphics::Event& event) override;
    virtual void onPress    (Graphics::RenderWindow& window, const Graphics::Event& event) override;
    virtual void onHover    (Graphics::RenderWindow& window, const Graphics::Event& event) override;
    virtual void onUnhover  (Graphics::RenderWindow& window, const Graphics::Event& event) override;

private:
    void animationCtor();
    void animateOnHover();
    void animateOnUnhover();
    void animationCtorOnUnrelease();
};

} // namespace Gui

#endif