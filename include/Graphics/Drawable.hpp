#ifndef GRAPHICS_DRAWABLE_HPP
#define GRAPHICS_DRAWABLE_HPP

#include "Graphics/GraphicsWindow.hpp"

namespace Graphics
{

class Drawable
{
public:
    virtual ~Drawable() = default;

    virtual void draw(RenderWindow& window) = 0;
};

} // namespace Graphics

#endif // GRAPHICS_DRAWABLE_HPP