#include "Gui/WindowContainer.hpp"

namespace Gui
{

WindowContainer::IteratorType WindowContainer::addWindow(std::unique_ptr<Window>&& window)
{
    windows_.push_back(std::move(window));

    return --windows_.end();
}

void WindowContainer::removeWindow(WindowContainer::IteratorType it)
{
    windows_.erase(it);
}

size_t WindowContainer::size() const
{
    return windows_.size();
}

} // namespace Gui