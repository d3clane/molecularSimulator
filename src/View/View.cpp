#include "View/View.hpp"
#include "Gui/Button.hpp"
#include "View/Gui/ButtonActions.hpp"

namespace View
{

namespace 
{

Graphics::Sprite loadSprite(std::vector<std::unique_ptr<Graphics::Texture > >& textures, const char* fileName)
{
    Graphics::Texture* texture = new Graphics::Texture{};
    texture->loadFromFile(fileName);

    Graphics::Sprite sprite;
    sprite.setTexture(*texture);

    textures.push_back(std::make_unique<Graphics::Texture>(texture));

    return sprite;
}

} // namespace anon

View::View(
    Model::MoleculeManager& manager, Graphics::RenderWindow& renderWindow,
    Engine::CoordsSystem& coordsSystem
) : coordsSystem_(coordsSystem), renderWindow_(renderWindow), manager_(manager)
{
    Graphics::Sprite addMoleculesSprite    = loadSprite(textures_, "media/textures/addMolecules.png");
    Graphics::Sprite removeMoleculesSprite = loadSprite(textures_, "media/textures/removeMolecules.png");

    static const unsigned int buttonWidth  = 64;
    static const unsigned int buttonHeight = 32;

    Gui::Button::CtorParams staticParams{
        {0, 0}, buttonWidth, buttonHeight, true, Gui::Button::State::Normal, 
        addMoleculesSprite, addMoleculesSprite, addMoleculesSprite, addMoleculesSprite
    };

    std::unique_ptr<Gui::Button> addMoleculesButton{
        new Gui::Button{{0, 0}, addMoleculesSprite, staticParams}
    };

    std::unique_ptr<Gui::Button> removeMoleculesButton{
        new Gui::Button{{buttonWidth, 0}, removeMoleculesSprite, staticParams}
    };

    ChangeMoleculesQuantityAction addMoleculesAction{
        manager_, Model::MoleculeType::Circle, ChangeMoleculesQuantityAction::ActionType::Add
    };

    ChangeMoleculesQuantityAction removeMoleculesAction{
        manager_, Model::MoleculeType::Circle, ChangeMoleculesQuantityAction::ActionType::Remove
    };

    addMoleculesButton.get()->addAction(&addMoleculesAction);



    windowManager_.addWindow(std::move(addMoleculesButton   ));
    windowManager_.addWindow(std::move(removeMoleculesButton));
}

void View::update(const Graphics::Event& event)
{
    windowManager_.update(renderWindow_, event);
}

void View::draw()
{
    windowManager_.draw(renderWindow_, coordsSystem_);
}

Gui::WindowManager& View::windowManager() &
{
    return windowManager_;
}

} // namespace View