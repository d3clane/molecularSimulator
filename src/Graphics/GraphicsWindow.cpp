#include "Graphics/Events.hpp"
#include "Graphics/GraphicsWindow.hpp"
#include "Graphics/PixelsArray.hpp"
#include "Graphics/Sprite.hpp"

#include "PrivateConverters.hpp"

namespace Graphics
{

RenderWindow::RenderWindow(int width, int height, const char* title) : 
        renderWindow_(sf::VideoMode(width, height), title), height_(height), width_(width)
{
}

void RenderWindow::display()
{
    renderWindow_.display();
}

bool RenderWindow::isOpen()
{
    return renderWindow_.isOpen();
}

void RenderWindow::close()
{
    renderWindow_.close();
}

void RenderWindow::clear()
{
    renderWindow_.clear();
}

void RenderWindow::drawLine(WindowLine line)
{
    sf::Vertex drewLine[] =
    {
        sf::Vertex(sf::Vector2f(line.start.x, line.start.y)),
        sf::Vertex(sf::Vector2f(line.end.x,   line.end.y  )),
    };

    renderWindow_.draw(drewLine, sizeof(drewLine) / sizeof(*drewLine), sf::Lines);
}

void RenderWindow::drawPixels(const PixelsArray& pixels)
{
    sf::Texture texture;
    texture.create(pixels.width_, pixels.height_);
    sf::Sprite sprite;
    
    texture.update(pixels.pixels_);
    sprite.setTexture(texture);

    renderWindow_.draw(sprite);
}

void RenderWindow::drawSprite(const Sprite& sprite)
{
    renderWindow_.draw(sprite.sprite_);
}

unsigned int RenderWindow::getWidth () const
{
    return width_;
}

unsigned int RenderWindow::getHeight() const
{
    return height_;
}

bool RenderWindow::pollEvent(Event& event)
{
    bool anyEvent = renderWindow_.pollEvent(event.sfEvent);

    if (!anyEvent) return anyEvent;

    switch (event.sfEvent.type)
    {
        case sf::Event::Closed:
            event.type = Event::EventType::Closed;
            break;

        case sf::Event::KeyPressed:
            event.type = Event::EventType::KeyPressed;
            event.keyCode = convertSfKeyCodeToKeyCode(event.sfEvent.key.code);
            break;
        
        case sf::Event::MouseMoved:
            event.type = Event::EventType::MouseMove;
            event.mousePos = WindowPoint(event.sfEvent.mouseMove.x, event.sfEvent.mouseMove.y);
            break;

        case sf::Event::MouseButtonPressed:
            event.type = Event::EventType::MouseButtonPressed;
            event.mouseButton = convertSfButtonEventToButtonEvent(event.sfEvent.mouseButton);
            break;

        case sf::Event::MouseButtonReleased:
            event.type = Event::EventType::MouseButtonReleased;
            event.mouseButton = convertSfButtonEventToButtonEvent(event.sfEvent.mouseButton);
            break;
            
        default:
            event.type = Event::EventType::None;
            break;
    }

    return anyEvent;
}

} // Graphics