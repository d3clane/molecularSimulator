#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <SFML/Graphics.hpp>
#include "GraphicsWindow.hpp"
#include "PixelsArray.hpp"

namespace Graphics
{

class Circle
{
    sf::CircleShape circle_;

public:
    Circle(float radius);

    void setCenterPos(int x, int y);
    WindowPoint getCenterPos();

    void setColor(const Color& color);
    Color getColor();

    friend void Graphics::RenderWindow::drawCircle(const Circle& circle);
};

} // namespace Graphics

#endif // CIRCLE_HPP