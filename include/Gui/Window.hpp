#ifndef GUI_WINDOW_HPP
#define GUI_WINDOW_HPP

#include "Graphics/Updatable.hpp"
#include "Scene/Renderable.hpp"

namespace Gui
{

class Window : public Scene::Renderable, public Graphics::Updatable
{
};

};

#endif // GUI_WINDOW_HPP