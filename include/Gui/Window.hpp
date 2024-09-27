#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include "Graphics/Events.hpp"

#include "Graphics/Drawable.hpp"

namespace Gui
{

class Window : public Graphics::Drawable
{
public:
    virtual bool update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event) = 0;
};


};

#endif // GUI_WINDOW_HPP