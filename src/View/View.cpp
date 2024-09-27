#include "View/View.hpp"
#include "Gui/Button.hpp"

namespace View
{

//TODO: it's temporary solution

#define TEXTURE_LOAD(FILE_NAME, TEXTURE_NAME, SPRITE_NAME)      \ 
    Graphics::Texture TEXTURE_NAME;                      \
    Graphics::Sprite  SPRITE_NAME;                       \
    TEXTURE_NAME.loadFromFile(FILE_NAME);                       \
    SPRITE_NAME.setTexture(TEXTURE_NAME); 

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