#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include "Graphics/Updatable.hpp"
#include "Scene/Drawable.hpp"

namespace Gui
{

class Window : public Scene::Drawable, public Graphics::Updatable
{
};

};

#endif // GUI_WINDOW_HPP