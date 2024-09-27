#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include "Graphics/Drawable.hpp"
#include "Graphics/Updatable.hpp"

namespace Gui
{

class Window : public Graphics::Drawable, public Graphics::Updatable
{
};

};

#endif // GUI_WINDOW_HPP