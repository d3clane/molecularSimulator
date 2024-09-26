#ifndef GRAPHICS_DRAWABLE_HPP
#define GRAPHICS_DRAWABLE_HPP

#include "Graphics/Sprite.hpp"
#include "Engine/CoordsSystem.hpp"

namespace Scene
{

class Drawable
{
public:
    virtual void draw(Graphics::RenderWindow& window, const Engine::CoordsSystem& coordsSystem) = 0;
};

} // namespace Scene

#endif // GRAPHICS_DRAWABLE_HPP