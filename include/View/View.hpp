#ifndef VIEW_VIEW_HPP
#define VIEW_VIEW_HPP

#include "Gui/WindowManager.hpp"

#include "Engine/CoordsSystem.hpp"
#include "Controller/Controller.hpp"

#include "Graphics/Texture.hpp"
#include "Graphics/Font.hpp"

#include "Scene/Renderable.hpp"

namespace Simulator
{

class View
{
    Graphics::RenderWindow& renderWindow_;
    Engine::CoordsSystem&   coordsSystem_;
    Simulator::Controller& controller_;

    Gui::WindowManager windowManager_;

    std::vector<std::unique_ptr<Graphics::Texture> > textures_;
    std::vector<std::unique_ptr<Graphics::Font> >    fonts_;
    
    Graphics::Sprite moleculeSprites_[Simulator::numberOfDifferentMolecules];

    std::vector<std::unique_ptr<Graphics::Renderable> > graphicsRenderables_; 

public:
    View(
        Simulator::Controller& controller, Graphics::RenderWindow& renderWindow, 
        Engine::CoordsSystem& coordsSystem
    );

    void update(const Graphics::Event& event);

    void draw();

    Gui::WindowManager& windowManager() &;
};

} // namespace Simulator

#endif // VIEW_VIEW_HPP