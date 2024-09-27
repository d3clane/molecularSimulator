#include "Gui/WindowManager.hpp"

namespace Gui
{

void WindowManager::update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    for (auto& window : windows_)
    {
        window->update(renderWindow, event);
    }
}

} // namespace Gui