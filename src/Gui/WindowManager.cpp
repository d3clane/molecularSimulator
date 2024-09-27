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

} // namespace Gui