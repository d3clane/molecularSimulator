#ifndef SCENE_DRAWABLE_HPP
#define SCENE_DRAWABLE_HPP

#include "Graphics/Sprite.hpp"
#include "Engine/CoordsSystem.hpp"

namespace Scene
{

class Renderable
{
public:
    virtual void draw(Graphics::RenderWindow& window, const Engine::CoordsSystem& coordsSystem) = 0;

    virtual ~Renderable() = default;
};

} // namespace Scene

#endif // SCENE_DRAWABLE_HPP