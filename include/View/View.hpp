#ifndef VIEW_VIEW_HPP
#define VIEW_VIEW_HPP

#include "Model/MoleculeManager.hpp" // TODO: move to controller
#include "Gui/WindowManager.hpp"     // stay here

#include "Engine/CoordsSystem.hpp"

namespace View
{

class View
{
    Graphics::RenderWindow& renderWindow_;
    Engine::CoordsSystem&   coordsSystem_;
    Model::MoleculeManager& manager_; // will be a controller

    Gui::WindowManager windowManager_; // always have as a view

    std::vector<std::unique_ptr<Graphics::Texture> > textures_;
    
    Graphics::Sprite moleculeSprites[Model::numberOfDifferentMolecules];
public:
    View(
        Model::MoleculeManager& manager, Graphics::RenderWindow& renderWindow, 
        Engine::CoordsSystem& coordsSystem
    );

    void update(const Graphics::Event& event);

    void draw();

    Gui::WindowManager& windowManager() &;
};

} // namespace View

#endif // VIEW_VIEW_HPP