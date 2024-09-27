#include "View/View.hpp"
#include "Gui/Button.hpp"

namespace View
{

//TODO: it's temporary solution

#define TEXTURE_LOAD(FILE_NAME, TEXTURE_NAME, SPRITE_NAME)      \ 
    static Graphics::Texture TEXTURE_NAME;                      \
    static Graphics::Sprite  SPRITE_NAME;                       \
    TEXTURE_NAME.loadFromFile(FILE_NAME);                       \
    SPRITE_NAME.setTexture(TEXTURE_NAME); 

View::View(Model::MoleculeManager& manager, Graphics::RenderWindow& renderWindow) :
    renderWindow_(renderWindow), manager_(manager)
{
    TEXTURE_LOAD("media/textures/add.png", addTexture, addSprite);

    std::unique_ptr<Gui::Button> addButton{
        new Gui::Button{
            {0, 0}, addSprite, Gui::Button::CtorParams{
                {0, 0}, 64, 32, true, Gui::Button::State::Normal, addSprite, addSprite, addSprite, addSprite
            }
        }
    };

    windowManager_.addWindow(std::move(addButton));
}

void View::update(const Graphics::Event& event)
{
    windowManager_.update(renderWindow_, event);
}

void View::draw()
{

}
void handleEvents();

Gui::WindowManager& windowManager() &;

} // namespace View