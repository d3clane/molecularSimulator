#ifndef VIEW_VIEW_HPP
#define VIEW_VIEW_HPP

#include "Model/MoleculeManager.hpp" // TODO: move to controller
#include "Gui/WindowManager.hpp"     // stay here

namespace View
{

class View
{
    Graphics::RenderWindow& renderWindow_;
    Model::MoleculeManager& manager_;

    Gui::WindowManager windowManager_;

public:
    View(Model::MoleculeManager& manager, Graphics::RenderWindow& window);

    void update(const Graphics::Event& event);

    void draw();
    void handleEvents();

    Gui::WindowManager& windowManager() &;
};

} // namespace View

#endif // VIEW_VIEW_HPP