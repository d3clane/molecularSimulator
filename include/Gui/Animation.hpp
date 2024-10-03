#ifndef GUI_ANIMATION_HPP
#define GUI_ANIMATION_HPP

#include "Graphics/Sprite.hpp"
#include "Scene/Renderable.hpp"

namespace Gui
{

class Animation final : public Scene::Renderable
{
    enum class AnimationDirection
    {
        FromBeginToEnd =  1,
        FromEndToBegin = -1,
    };

    struct AnimationSpritesAlpha
    {
        uint8_t animationBeginSpriteAlpha_;
        uint8_t animationEndSpriteAlpha_;
    };

    const Graphics::Sprite* animationBeginSprite_, *animationEndSprite_;
    AnimationDirection animationDirection_;

    std::chrono::milliseconds animationDuration_;

    uint8_t prevBeginSpriteAlpha_;
    std::chrono::steady_clock::time_point prevTime_;

public:
    Animation(
        const Graphics::Sprite* begin, const Graphics::Sprite* end,
        const std::chrono::milliseconds& duration
    );

    // When called for the first time will begin animation
    void swapAnimationDirection();

    void draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs) override;

private:
    AnimationSpritesAlpha animate();
};

} // namespace Gui

#endif // GUI_ANIMATION_HPP