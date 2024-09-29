#include "View/View.hpp"
#include "Gui/Button.hpp"
#include "View/Gui/ButtonActions.hpp"

#include "Gui/Action.hpp"

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

    textures.push_back(std::unique_ptr<Graphics::Texture>(texture));

    return sprite;
}


void drawMolecule(
    const Model::Molecule* molecule, Graphics::RenderWindow& renderWindow,
    const Engine::CoordsSystem& coordsSystem, 
    const Graphics::Sprite (*moleculeSprites)[Model::numberOfDifferentMolecules]
)
{
    Graphics::Sprite moleculeSprite = (*moleculeSprites)[(size_t)molecule->id()];

    moleculeSprite.setPosition(coordsSystem.getPosInWindow(molecule->pos()));

    switch (molecule->id())
    {
        case Model::MoleculeType::Circle:
        {
            const Model::CircleMolecule* circleMolecule = dynamic_cast<const Model::CircleMolecule*>(molecule);

            double radius = coordsSystem.getSizeInPixels(circleMolecule->radius());

            moleculeSprite.scaleInPixels({radius * 2, radius * 2});

            break;
        }
        case Model::MoleculeType::Rectangle:
        {
            const Model::RectangleMolecule* rectangleMolecule = 
                dynamic_cast<const Model::RectangleMolecule*>(molecule);

            double width  = coordsSystem.getSizeInPixels(rectangleMolecule->width());
            double height = coordsSystem.getSizeInPixels(rectangleMolecule->height());

            moleculeSprite.scaleInPixels({width, height});

            break;
        }
        default:
            break;
    }

    renderWindow.drawSprite(moleculeSprite);
}

} // namespace anon

View::View(
    Model::MoleculeManager& manager, Graphics::RenderWindow& renderWindow,
    Engine::CoordsSystem& coordsSystem
) : coordsSystem_(coordsSystem), renderWindow_(renderWindow), manager_(manager)
{
    Graphics::Sprite addMoleculesSprite    = loadSprite(textures_, "media/textures/plus.jpeg");
    Graphics::Sprite removeMoleculesSprite = loadSprite(textures_, "media/textures/minus.jpeg");

    static const unsigned int buttonWidth  = 64;
    static const unsigned int buttonHeight = 32;

    Gui::Button::CtorParams staticParams{
        {0, 0, 0}, buttonWidth, buttonHeight, true, Gui::Button::State::Normal, 
        addMoleculesSprite, addMoleculesSprite, addMoleculesSprite, addMoleculesSprite
    };

    std::unique_ptr<Gui::Button> addMoleculesButton{
        new Gui::Button{{0, 0, 0}, addMoleculesSprite, staticParams}
    };

    std::unique_ptr<Gui::Button> removeMoleculesButton{
        new Gui::Button{{buttonWidth, 0, 0}, removeMoleculesSprite, staticParams}
    };

    auto* addMoleculesAction = new ChangeMoleculesQuantityAction{
        manager_, Model::MoleculeType::Circle, ChangeMoleculesQuantityAction::ActionType::Add
    };

    auto* removeMoleculesAction = new ChangeMoleculesQuantityAction{
        manager_, Model::MoleculeType::Circle, ChangeMoleculesQuantityAction::ActionType::Remove
    };

    addMoleculesButton.get()->addAction(std::unique_ptr<Gui::Action>(addMoleculesAction));
    removeMoleculesButton.get()->addAction(std::unique_ptr<Gui::Action>(removeMoleculesAction));

    windowManager_.addWindow(std::move(addMoleculesButton   ));
    windowManager_.addWindow(std::move(removeMoleculesButton));

    moleculeSprites[(size_t)Model::MoleculeType::Circle] = 
        loadSprite(textures_, "media/textures/whiteCircle.png");

    moleculeSprites[(size_t)Model::MoleculeType::Rectangle] = 
        loadSprite(textures_, "media/textures/red.jpeg");
}

void View::update(const Graphics::Event& event)
{
    windowManager_.update(renderWindow_, event);
}

void View::draw()
{
    windowManager_.draw(renderWindow_, coordsSystem_);

    std::list<std::unique_ptr<Model::Molecule> >& molecules = manager_.molecules();

    for (auto& molecule : molecules)
    {
        drawMolecule(molecule.get(), renderWindow_, coordsSystem_, &moleculeSprites);
    }
}

Gui::WindowManager& View::windowManager() &
{
    return windowManager_;
}

} // namespace View