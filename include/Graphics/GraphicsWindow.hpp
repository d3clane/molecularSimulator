#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include "Mouse.hpp"

namespace Graphics
{

class Event;

struct WindowPoint
{
    int x, y;

    WindowPoint(int x, int y) : x(x), y(y) {}
};

inline WindowPoint operator +(const WindowPoint& left, const WindowPoint& right)
{
    return WindowPoint(left.x + right.x, left.y + right.y);
}

inline WindowPoint operator -(const WindowPoint& point) 
{   
    return WindowPoint(-point.x, -point.y);
}

inline WindowPoint operator -(const WindowPoint& left, const WindowPoint& right)
{
    return WindowPoint(left.x - right.x, left.y - right.y);
}

using WindowVector = WindowPoint;

struct WindowLine
{
    WindowPoint start, end;

    WindowLine(const WindowPoint& start, const WindowPoint& end) : start(start), end(end) {}
};

class PixelsArray;
class Sprite;

class Window
{
private:
    sf::RenderWindow window_;

public:
    const unsigned int width_, height_;

    Window(int width, int height, const char* title);

    bool isOpen();
    void close();
    void clear();

    void drawLine  (WindowLine line);
    void drawPixels(const PixelsArray& pixels);
    void drawSprite(const Sprite& sprite);

    void display();

    unsigned int getWidth () const;
    unsigned int getHeight() const;

    bool pollEvent(Event& event);

    friend WindowPoint Mouse::getPosition(const Window& window);
};

} // Graphics

#endif // GRAPHICS_HPP