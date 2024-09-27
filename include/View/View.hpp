#ifndef VIEW_VIEW_HPP
#define VIEW_VIEW_HPP

#include "Model/MoleculeManager.hpp" // TODO: move to controller
#include "Gui/WindowManager.hpp"     // stay here

namespace View
{

class View
{
    Graphics::RenderWindow& window_;
    Model::MoleculeManager& manager_;

public:
    View(Model::MoleculeManager& manager, Graphics::RenderWindow& window);

    void update();

    void draw();
    void handleEvents();
};

} // namespace View

#endif // VIEW_VIEW_HPP