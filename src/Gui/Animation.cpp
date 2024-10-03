#include "Gui/Animation.hpp"
#include "Graphics/PixelsArray.hpp"

#include <cstdio>
#include <iostream>

namespace Gui
{

namespace 
{

void setAlphaColorOfSprite(Graphics::Sprite& sprite, uint8_t alpha)
{
    Graphics::Color color = sprite.getColor();
    sprite.setColor({color.red_, color.green_, color.blue_, alpha});
}

} // namespace anon

Animation::Animation(
    const Graphics::Sprite* begin, const Graphics::Sprite* end,
    const std::chrono::milliseconds& duration
) : animationBeginSprite_(begin), animationEndSprite_(end), 
    animationDirection_(AnimationDirection::FromEndToBegin), animationDuration_(duration)
{
    prevTime_ = std::chrono::steady_clock::now();
    prevBeginSpriteAlpha_ = Graphics::Color::maxColorValue_;
}

void Animation::swapAnimationDirection()
{
    prevBeginSpriteAlpha_ = animate().animationBeginSpriteAlpha_;

    animationDirection_ = (animationDirection_ == AnimationDirection::FromBeginToEnd) ? 
        AnimationDirection::FromEndToBegin : AnimationDirection::FromBeginToEnd;

    prevTime_ = std::chrono::steady_clock::now();
}

Animation::AnimationSpritesAlpha Animation::animate()
{
    using Ms = std::chrono::milliseconds;

    auto timeNow = std::chrono::steady_clock::now();

    uint8_t maxAlphaVal = Graphics::Color::maxColorValue_;
    uint8_t deltaAlpha = std::min(
        static_cast<double>(maxAlphaVal), 
        maxAlphaVal * static_cast<double>(std::chrono::duration_cast<Ms>(timeNow - prevTime_).count()) / 
            animationDuration_.count()
    );

    assert(static_cast<int>(animationDirection_) == -1 || static_cast<int>(animationDirection_) == 1);
    assert(static_cast<int>(AnimationDirection::FromBeginToEnd) ==  1);
    assert(static_cast<int>(AnimationDirection::FromEndToBegin) == -1);

    uint8_t beginSpriteAlpha = std::min(
        static_cast<int>(maxAlphaVal),
        std::max(0, static_cast<int>(prevBeginSpriteAlpha_) - deltaAlpha * static_cast<int>(animationDirection_))
    );

    return {beginSpriteAlpha, static_cast<uint8_t>(maxAlphaVal - beginSpriteAlpha)};
}

void Animation::draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs)
{
    Graphics::Sprite firstLayerSprite  = *animationBeginSprite_;
    Graphics::Sprite secondLayerSprite = *animationEndSprite_;

    Animation::AnimationSpritesAlpha animationSpritesAlpha = animate();

    //fprintf(stderr, "Animation: %d %d\n", animationSpritesAlpha.animationBeginSpriteAlpha_, animationSpritesAlpha.animationEndSpriteAlpha_);
    setAlphaColorOfSprite(firstLayerSprite,  animationSpritesAlpha.animationBeginSpriteAlpha_);
    setAlphaColorOfSprite(secondLayerSprite, animationSpritesAlpha.animationEndSpriteAlpha_  );

    renderWindow.drawSprite(firstLayerSprite);
    renderWindow.drawSprite(secondLayerSprite);
}

} // namespace Gui