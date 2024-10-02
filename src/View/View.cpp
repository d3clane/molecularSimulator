#include "View/View.hpp"
#include "Gui/Button.hpp"
#include "View/Gui/ButtonActions.hpp"
#include "View/Gui/GraphsWindow.hpp"

#include "Gui/Action.hpp"

namespace Simulator
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

const Graphics::Font* loadFont(std::vector<std::unique_ptr<Graphics::Font> >& fonts, const char* fileName)
{
    Graphics::Font* font = new Graphics::Font{};
    font->loadFromFile(fileName);

    fonts.push_back(std::unique_ptr<Graphics::Font>(font));

    return font;
}

void drawMolecule(
    const Simulator::Molecule* molecule, Graphics::RenderWindow& renderWindow,
    const Engine::CoordsSystem& coordsSystem, 
    const Graphics::Sprite (*moleculeSprites)[Simulator::numberOfDifferentMolecules]
)
{
    Graphics::Sprite moleculeSprite = (*moleculeSprites)[(size_t)molecule->id()];

    moleculeSprite.setPosition(coordsSystem.getPointInWindow(molecule->pos()));

    switch (molecule->id())
    {
        case Simulator::MoleculeType::Circle:
        {
            const Simulator::CircleMolecule* circleMolecule = 
                static_cast<const Simulator::CircleMolecule*>(molecule);

            double radius = coordsSystem.getSizeInPixels(circleMolecule->radius());

            moleculeSprite.scaleInPixels({radius * 2, radius * 2});

            break;
        }
        case Simulator::MoleculeType::Rectangle:
        {
            const Simulator::RectangleMolecule* rectangleMolecule = 
                static_cast<const Simulator::RectangleMolecule*>(molecule);

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
    Simulator::Controller& controller, Graphics::RenderWindow& renderWindow,
    Engine::CoordsSystem& coordsSystem
) : coordsSystem_(coordsSystem), renderWindow_(renderWindow), controller_(controller)
{
    controller_.addBoundary(Simulator::Boundary(Engine::Point{0, 0, 0}, 0, 600,   Engine::Vector(1, 0, 0)));
    controller_.addBoundary(Simulator::Boundary(Engine::Point{0, 0, 0}, 800, 0,   Engine::Vector(0, 1, 0)));
    controller_.addBoundary(Simulator::Boundary(Engine::Point{0, 600, 0}, 800, 0, Engine::Vector(0, -1, 0)));
    controller_.addBoundary(Simulator::Boundary(Engine::Point{800, 0, 0}, 0, 600, Engine::Vector(-1, 0, 0)));
    
    Graphics::Sprite addMoleculesSprite    = loadSprite(textures_, "media/textures/plus.jpeg");
    Graphics::Sprite removeMoleculesSprite = loadSprite(textures_, "media/textures/minus.jpeg");

    const Graphics::Font* font = loadFont(fonts_, "media/fonts/arial.ttf");

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
        controller_, Simulator::MoleculeType::Circle, ChangeMoleculesQuantityAction::ActionType::Add
    };

    auto* removeMoleculesAction = new ChangeMoleculesQuantityAction{
        controller_, Simulator::MoleculeType::Circle, ChangeMoleculesQuantityAction::ActionType::Remove
    };

    addMoleculesButton.get()->addAction(std::unique_ptr<Gui::Action>(addMoleculesAction));
    removeMoleculesButton.get()->addAction(std::unique_ptr<Gui::Action>(removeMoleculesAction));

    windowManager_.addWindow(std::move(addMoleculesButton   ));
    windowManager_.addWindow(std::move(removeMoleculesButton));

    moleculeSprites[(size_t)Simulator::MoleculeType::Circle] = 
        loadSprite(textures_, "media/textures/whiteCircle.png");

    moleculeSprites[(size_t)Simulator::MoleculeType::Rectangle] = 
        loadSprite(textures_, "media/textures/red.jpeg");


    Engine::CoordsSystem temperatureCs{{1, 0, 0}, {0, -1, 0}, {0, 0, 1}, {1, 600, 0}};

    auto* temperatureGraphsWindow = new Simulator::TemperatureGraphsWindow{
        temperatureCs, {0, 0, 0}, 200, 300, std::chrono::milliseconds(10), 1, controller
    };

    auto temperatureGraphTemperature = new Graphics::Text{};
    auto temperatureGraphTime        = new Graphics::Text{};

    // TODO: 100% create text in scene with coords system

    temperatureGraphTemperature->setFont(*font);
    temperatureGraphTemperature->setString("Temperature");
    temperatureGraphTemperature->setPosition({1, 300});

    temperatureGraphTime->setFont(*font);
    temperatureGraphTime->setString("Time");
    temperatureGraphTime->setPosition({201, 550});

    graphicsRenderables_.push_back(std::unique_ptr<Graphics::Renderable>(temperatureGraphTemperature));
    graphicsRenderables_.push_back(std::unique_ptr<Graphics::Renderable>(temperatureGraphTime));

    windowManager_.addWindow(std::unique_ptr<Gui::Window>(temperatureGraphsWindow));
}

void View::update(const Graphics::Event& event)
{
    windowManager_.update(renderWindow_, event);
}

void View::draw()
{
    for (auto& renderable : graphicsRenderables_)
    {
        renderable.get()->draw(renderWindow_);
    }

    windowManager_.draw(renderWindow_, coordsSystem_);

    std::list<std::unique_ptr<Simulator::Molecule> >& molecules = controller_.molecules();

    for (auto& molecule : molecules)
    {
        drawMolecule(molecule.get(), renderWindow_, coordsSystem_, &moleculeSprites);
    }
}

Gui::WindowManager& View::windowManager() &
{
    return windowManager_;
}

} // namespace Simulator