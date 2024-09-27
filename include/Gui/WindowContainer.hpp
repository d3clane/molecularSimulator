#ifndef GUI_WINDOW_CONTAINER_HPP
#define GUI_WINDOW_CONTAINER_HPP

#include <list>
#include <memory>

#include "Gui/Window.hpp"

namespace Gui
{

class WindowContainer : public Window
{
protected:
    std::list<std::unique_ptr<Window>> windows_;

public:
    using IteratorType = std::list<std::unique_ptr<Window>>::iterator;

    IteratorType addWindow(std::unique_ptr<Window>&& window);
    void removeWindow(IteratorType it);

    size_t size() const;
};

} // namespace Gui

#endif // GUI_WINDOW_CONTAINER_HPP