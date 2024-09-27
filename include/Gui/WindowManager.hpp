#ifndef GUI_WINDOW_MANAGER_HPP
#define GUI_WINDOW_MANAGER_HPP

#include "Gui/WindowContainer.hpp"

namespace Gui
{

class WindowManager : public WindowContainer
{
public:
    virtual void update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
};

} // namespace Gui


#endif // GUI_WINDOW_MANAGER_HPP