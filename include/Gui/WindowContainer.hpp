#ifndef GUI_WINDOW_CONTAINER_HPP
#define GUI_WINDOW_CONTAINER_HPP

#include <list>
#include <memory>

#include "Gui/Window.hpp"
#include "Gui/WindowManager.hpp"

namespace Gui
{

class WindowContainer : public Window, public WindowManager
{
public:
    virtual bool update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event) = 0;
};

} // namespace Gui

#endif // GUI_WINDOW_CONTAINER_HPP