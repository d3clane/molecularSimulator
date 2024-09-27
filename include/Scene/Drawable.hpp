#ifndef SCENE_DRAWABLE_HPP
#define SCENE_DRAWABLE_HPP

#include "Graphics/Sprite.hpp"
#include "Engine/CoordsSystem.hpp"

namespace Scene
{

class Drawable
{
public:
    virtual void draw(Graphics::RenderWindow& window, const Engine::CoordsSystem& coordsSystem) = 0;

    virtual ~Drawable() = default;
};

} // namespace Scene

#endif // SCENE_DRAWABLE_HPP