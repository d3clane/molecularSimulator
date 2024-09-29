#ifndef VIEW_VIEW_HPP
#define VIEW_VIEW_HPP

#include "Gui/WindowManager.hpp"

#include "Engine/CoordsSystem.hpp"
#include "Controller/Controller.hpp"

namespace Simulator
{

class View
{
    Graphics::RenderWindow& renderWindow_;
    Engine::CoordsSystem&   coordsSystem_;
    Simulator::Controller& controller_;

    Gui::WindowManager windowManager_;

    std::vector<std::unique_ptr<Graphics::Texture> > textures_;
    
    Graphics::Sprite moleculeSprites[Simulator::numberOfDifferentMolecules];
public:
    View(
        Simulator::Controller& controller, Graphics::RenderWindow& renderWindow, 
        Engine::CoordsSystem& coordsSystem
    );

    void update(const Graphics::Event& event);

    void draw();

    Gui::WindowManager& windowManager() &;
};

} // namespace View

#endif // VIEW_VIEW_HPP