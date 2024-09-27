#include "Gui/WindowManager.hpp"

namespace Gui
{

bool WindowManager::update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    bool updated = false;
    for (auto& window : windows_)
    {
        updated |= window->update(renderWindow, event);
    }

    return updated;
}

WindowManager::IteratorType WindowManager::addWindow(std::unique_ptr<Window>&& window)
{
    windows_.push_back(std::move(window));

    return --windows_.end();
}

void WindowManager::removeWindow(WindowManager::IteratorType it)
{
    windows_.erase(it);
}

size_t WindowManager::size() const
{
    return windows_.size();
}

} // namespace Gui