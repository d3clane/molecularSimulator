#include "View/View.hpp"
#include "Gui/Button.hpp"
#include "View/Gui/ButtonActions.hpp"
#include "View/Gui/GraphsWindow.hpp"

#include "Gui/Action.hpp"

#include "Utils/Exceptions.hpp"
#include <iostream>

namespace Simulator
{

namespace 
{

Graphics::Sprite loadSprite(std::vector<std::unique_ptr<Graphics::Texture> >& textures, const char* fileName)
{
    assert(fileName);

    Graphics::Texture* texture = new Graphics::Texture{};
    bool loadingSuccess = texture->loadFromFile(fileName);

    if (!loadingSuccess)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::LoadingFromFileErr,
            "can't load texture from file",
            nullptr
        );
    }

    Graphics::Sprite sprite;
    sprite.setTexture(*texture);

    textures.push_back(std::unique_ptr<Graphics::Texture>(texture));

    return sprite;
}

const Graphics::Font* loadFont(std::vector<std::unique_ptr<Graphics::Font> >& fonts, const char* fileName)
{
    assert(fileName);

    Graphics::Font* font = new Graphics::Font{};
    bool loadingSuccess = font->loadFromFile(fileName);

    if (!loadingSuccess)
    {
        throw EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(
            Utils::SimulatorErrors::LoadingFromFileErr,
            "can't load font from file",
            nullptr
        );
    }

    fonts.push_back(std::unique_ptr<Graphics::Font>(font));

    return font;
}

void drawMolecule(
    const Simulator::Molecule* molecule, Graphics::RenderWindow& renderWindow,
    const Engine::CoordsSystem& coordsSystem, 
    const Graphics::Sprite (*moleculesSprites)[Simulator::numberOfDifferentMolecules]
)
{
    assert(molecule);
    assert(moleculesSprites);

    Graphics::Sprite moleculeSprite = (*moleculesSprites)[(size_t)molecule->id()];

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

void addBoundaries(Simulator::Controller& controller, const size_t width, const size_t height)
{
    controller.addBoundary(Simulator::Boundary(Engine::Point{0, 0, 0}, 0, height,     Engine::Vector(1, 0, 0)));
    controller.addBoundary(Simulator::Boundary(Engine::Point{0, 0, 0}, width, 0,      Engine::Vector(0, 1, 0)));
    controller.addBoundary(Simulator::Boundary(Engine::Point{0, height, 0}, width, 0, Engine::Vector(0, -1, 0)));
    controller.addBoundary(Simulator::Boundary(Engine::Point{width, 0, 0}, 0, height, Engine::Vector(-1, 0, 0)));
}

void addButtons(
    Controller& controller,
    Gui::WindowManager& windowManager, const size_t width, const size_t height,
    std::vector<std::unique_ptr<Graphics::Texture> >& textures
)
{
    Graphics::Sprite addMoleculesSprite    = loadSprite(textures, "media/textures/plus.jpeg");
    Graphics::Sprite removeMoleculesSprite = loadSprite(textures, "media/textures/minus.jpeg");
    
    static const unsigned int buttonWidth  = 64;
    static const unsigned int buttonHeight = 64;

    Gui::Button::CtorParams staticParams{
        {0, 0, 0}, buttonWidth, buttonHeight, true, Gui::Button::State::Normal, 
        addMoleculesSprite, addMoleculesSprite, addMoleculesSprite, addMoleculesSprite
    };

    Gui::Button* addMoleculesButton{
        new Gui::Button{{0, 0, 0}, addMoleculesSprite, staticParams}
    };

    Gui::Button* removeMoleculesButton{
        new Gui::Button{{buttonWidth, 0, 0}, removeMoleculesSprite, staticParams}
    };

    auto* addMoleculesAction = new ChangeMoleculesQuantityAction{
        controller, Simulator::MoleculeType::Circle, ChangeMoleculesQuantityAction::ActionType::Add
    };

    auto* removeMoleculesAction = new ChangeMoleculesQuantityAction{
        controller, Simulator::MoleculeType::Circle, ChangeMoleculesQuantityAction::ActionType::Remove
    };

    addMoleculesButton->addAction(std::unique_ptr<Gui::Action>(addMoleculesAction));
    removeMoleculesButton->addAction(std::unique_ptr<Gui::Action>(removeMoleculesAction));

    windowManager.addWindow(std::unique_ptr<Gui::Button>(addMoleculesButton));
    windowManager.addWindow(std::unique_ptr<Gui::Button>(removeMoleculesButton));
}

void addMoleculesSprites(
    Graphics::Sprite (*moleculesSprites)[Simulator::numberOfDifferentMolecules],
    std::vector<std::unique_ptr<Graphics::Texture> >& textures
)
{
    (*moleculesSprites)[(size_t)Simulator::MoleculeType::Circle] = 
        loadSprite(textures, "media/textures/whiteCircle.png");

    (*moleculesSprites)[(size_t)Simulator::MoleculeType::Rectangle] = 
        loadSprite(textures, "media/textures/red.jpeg");
}

void addGraphs(
    Controller& controller, 
    Gui::WindowManager& windowManager, 
    std::vector<std::unique_ptr<Graphics::Font> >& fonts, 
    std::vector<std::unique_ptr<Graphics::Renderable> >& graphicsRenderables,
    const size_t width, const size_t height
)
{
    Engine::CoordsSystem temperatureCs{{1, 0, 0}, {0, -1, 0}, {0, 0, 1}, {1, height, 0}};

    static const size_t temperatureGraphsWindowHeight = 300;
    static const size_t temperatureGraphsWindowWidth  = 200;
    static const long long durationBetweenMeasurementsInMs = 50;
    static const double stepBetweenPoints = 5;
    static const Point temperatureGraphsBeginPoint{0, 0, 0};

    auto* temperatureGraphsWindow = new Simulator::TemperatureGraphsWindow{
        temperatureCs, temperatureGraphsBeginPoint, 
        temperatureGraphsWindowWidth, temperatureGraphsWindowHeight, 
        std::chrono::milliseconds(durationBetweenMeasurementsInMs), stepBetweenPoints, controller
    };

    auto temperatureGraphTemperature = new Graphics::Text{};
    auto temperatureGraphTime        = new Graphics::Text{};

    const Graphics::Font* font = loadFont(fonts, "media/fonts/arial.ttf");

    temperatureGraphTemperature->setFont(*font);
    temperatureGraphTemperature->setString("Temperature");
    temperatureGraphTemperature->setPosition({1, 300});

    temperatureGraphTime->setFont(*font);
    temperatureGraphTime->setString("Time");
    temperatureGraphTime->setPosition({201, 550});

    graphicsRenderables.push_back(std::unique_ptr<Graphics::Renderable>(temperatureGraphTemperature));
    graphicsRenderables.push_back(std::unique_ptr<Graphics::Renderable>(temperatureGraphTime));

    windowManager.addWindow(std::unique_ptr<Gui::Window>(temperatureGraphsWindow));
}

} // namespace anon

View::View(
    Simulator::Controller& controller, Graphics::RenderWindow& renderWindow,
    Engine::CoordsSystem& coordsSystem
) : coordsSystem_(coordsSystem), renderWindow_(renderWindow), controller_(controller)
{
    const size_t width = renderWindow.getWidth();
    const size_t height = renderWindow.getHeight();

    addBoundaries(controller_, width, height);
    addButtons(controller_, windowManager_, width, height, textures_);
    addMoleculesSprites(&moleculeSprites_, textures_);
    addGraphs(controller_, windowManager_, fonts_, graphicsRenderables_, width, height);
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
        drawMolecule(molecule.get(), renderWindow_, coordsSystem_, &moleculeSprites_);
    }
}

Gui::WindowManager& View::windowManager() &
{
    return windowManager_;
}

} // namespace Simulator