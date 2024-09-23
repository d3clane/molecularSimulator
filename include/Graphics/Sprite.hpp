#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SFML/Graphics.hpp>
#include <chrono>

#include "Graphics/GraphicsWindow.hpp"

namespace Graphics
{

class Texture;
class Color;

class Sprite
{
    sf::Sprite sprite_;

public:

    void setTexture (const Texture& texture);
    void setPosition(const WindowPoint& position);
    void scale      (const float scaleX, const float scaleY);

    void scaleInPixels(const WindowVector& scaleInPixels);

    WindowVector getScaleInPixels() const;

    void  setColor(const Color& color);
    Color getColor() const;

    friend void Window::drawSprite(const Sprite& sprite);
};


} // namespace Graphics

#endif // SPRITE_HPP