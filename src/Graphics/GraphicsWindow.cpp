#include "Graphics/Events.hpp"
#include "Graphics/GraphicsWindow.hpp"
#include "Graphics/PixelsArray.hpp"
#include "Graphics/Sprite.hpp"

#include "PrivateConverters.hpp"

namespace Graphics
{

Window::Window(int width, int height, const char* title) : 
        window_(sf::VideoMode(width, height), title), height_(height), width_(width)
{
}

void Window::display()
{
    window_.display();
}

bool Window::isOpen()
{
    return window_.isOpen();
}

void Window::close()
{
    window_.close();
}

void Window::clear()
{
    window_.clear();
}

void Window::drawLine(WindowLine line)
{
    sf::Vertex drewLine[] =
    {
        sf::Vertex(sf::Vector2f(line.start.x, line.start.y)),
        sf::Vertex(sf::Vector2f(line.end.x,   line.end.y  )),
    };

    window_.draw(drewLine, sizeof(drewLine) / sizeof(*drewLine), sf::Lines);
}

void Window::drawPixels(const PixelsArray& pixels)
{
    sf::Texture texture;
    texture.create(pixels.width_, pixels.height_);
    sf::Sprite sprite;
    
    texture.update(pixels.pixels_);
    sprite.setTexture(texture);

    window_.draw(sprite);
}

void Window::drawSprite(const Sprite& sprite)
{
    window_.draw(sprite.sprite_);
}

unsigned int Window::getWidth () const
{
    return width_;
}

unsigned int Window::getHeight() const
{
    return height_;
}

bool Window::pollEvent(Event& event)
{
    bool anyEvent = window_.pollEvent(event.sfEvent);

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