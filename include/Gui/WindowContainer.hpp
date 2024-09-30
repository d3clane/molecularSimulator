#ifndef GUI_WINDOW_CONTAINER_HPP
#define GUI_WINDOW_CONTAINER_HPP

#include <list>
#include <memory>

#include "Gui/Window.hpp"
#include "Gui/WindowManager.hpp"

namespace Gui
{

class WindowsContainer : public Window
{    
    std::list<std::unique_ptr<Window>> windows_;

public:
    using IteratorType = std::list<std::unique_ptr<Window>>::iterator;

    IteratorType addWindow(std::unique_ptr<Window>&& window);
    void removeWindow(IteratorType it);

    size_t size() const;

    virtual bool update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)   = 0;
    virtual void draw  (Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs) = 0;
};

} // namespace Gui

#endif // GUI_WINDOW_CONTAINER_HPP