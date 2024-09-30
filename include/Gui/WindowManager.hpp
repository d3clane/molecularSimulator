#ifndef GUI_WINDOW_MANAGER_HPP
#define GUI_WINDOW_MANAGER_HPP

#include "Gui/Window.hpp"
#include "Graphics/Updatable.hpp"
#include "Scene/Drawable.hpp"

#include <list>
#include <memory>

namespace Gui
{

class WindowManager final : public Graphics::Updatable, public Scene::Renderable
{
    std::list<std::unique_ptr<Window>> windows_;

public:
    using IteratorType = std::list<std::unique_ptr<Window>>::iterator;

    IteratorType addWindow(std::unique_ptr<Window>&& window);
    void removeWindow(IteratorType it);

    size_t size() const;

    virtual bool update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event  ) override;
    virtual void draw  (Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs) override;
};

} // namespace Gui


#endif // GUI_WINDOW_MANAGER_HPP