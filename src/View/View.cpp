#include "View/View.hpp"
#include "Gui/OnReleaseButton.hpp"
#include "Gui/HoverAnimatedButton.hpp"
#include "View/Gui/ButtonActions.hpp"
#include "View/Gui/GraphsWindow.hpp"

#include "Gui/Action.hpp"

#include "Utils/Exceptions.hpp"
#include <iostream>
#include <cassert>

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

void drawForcer(
    const Graphics::Sprite& forcerSprite, const Controller& controller,
    const Engine::CoordsSystem& coordsSystem, Graphics::RenderWindow& window
)
{
    Graphics::Sprite drawingSprite = forcerSprite;
    
    auto a = coordsSystem.getScaleInPixels(controller.getForcerScales());
    auto b = coordsSystem.getPointInWindow(controller.getForcerPos());

    drawingSprite.scaleInPixels(coordsSystem.getScaleInPixels(controller.getForcerScales()));
    drawingSprite.setPosition  (coordsSystem.getPointInWindow(controller.getForcerPos()));

    window.drawSprite(drawingSprite);
}

void addMoleculesButtons(
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

    Gui::OnReleaseButton* addMoleculesButton{
        new Gui::OnReleaseButton{{0, 0, 0}, addMoleculesSprite, staticParams}
    };

    Gui::OnReleaseButton* removeMoleculesButton{
        new Gui::OnReleaseButton{{buttonWidth, 0, 0}, removeMoleculesSprite, staticParams}
    };

    auto* addMoleculesAction = new ChangeMoleculesQuantityAction{
        controller, ChangeMoleculesQuantityAction::ActionType::Add
    };

    auto* removeMoleculesAction = new ChangeMoleculesQuantityAction{
        controller, ChangeMoleculesQuantityAction::ActionType::Remove
    };

    addMoleculesButton->addAction(std::unique_ptr<Gui::Action>(addMoleculesAction));
    removeMoleculesButton->addAction(std::unique_ptr<Gui::Action>(removeMoleculesAction));

    windowManager.addWindow(std::unique_ptr<Gui::Button>(addMoleculesButton));
    windowManager.addWindow(std::unique_ptr<Gui::Button>(removeMoleculesButton));
}

void addForcerButtons(
    Controller& controller,
    Gui::WindowManager& windowManager, const size_t renderWindowWidth, const size_t renderWindowHeight,
    std::vector<std::unique_ptr<Graphics::Texture> >& textures 
)
{
    Graphics::Sprite forcerUpSprite   = loadSprite(textures, "media/textures/up.jpeg");
    Graphics::Sprite forcerDownSprite = loadSprite(textures, "media/textures/down.jpeg");
    Graphics::Sprite hoveringSprite   = loadSprite(textures, "media/textures/red.jpeg");

    static const unsigned int buttonWidth  = 64;
    static const unsigned int buttonHeight = 64;

    Gui::Button::CtorParams staticParams{
        {0, 0, 0}, buttonWidth, buttonHeight, true, Gui::Button::State::Normal, 
        forcerUpSprite, hoveringSprite, forcerUpSprite, forcerUpSprite
    };

    static const std::chrono::milliseconds animationDuration{1000};

    auto* forcerUpButton = new Gui::HoverAnimatedButton{
        animationDuration, 
        Gui::Button::CtorParams{
            {renderWindowWidth - buttonWidth, 0, 0}, buttonWidth, buttonHeight, true, Gui::Button::State::Normal,
            forcerUpSprite, hoveringSprite, forcerUpSprite, forcerUpSprite
        }
    };

    auto* forcerDownButton = new Gui::HoverAnimatedButton{
        animationDuration, 
        Gui::Button::CtorParams{
            {renderWindowWidth - 2 * buttonWidth, 0, 0}, buttonWidth, buttonHeight, true, 
            Gui::Button::State::Normal,
            forcerDownSprite, hoveringSprite, forcerDownSprite, forcerDownSprite
        }
    };

    auto* moveUpAction = new MoveForcerAction{
        controller, MoveForcerAction::ActionType::MoveUp
    };
    auto* moveDownAction = new MoveForcerAction{
        controller, MoveForcerAction::ActionType::MoveDown
    };

    forcerUpButton->addAction(std::unique_ptr<Gui::Action>(moveUpAction));
    forcerDownButton->addAction(std::unique_ptr<Gui::Action>(moveDownAction));

    windowManager.addWindow(std::unique_ptr<Gui::Button>(forcerUpButton));
    windowManager.addWindow(std::unique_ptr<Gui::Button>(forcerDownButton));   
}

void addButtons(
    Controller& controller,
    Gui::WindowManager& windowManager, const size_t renderWindowWidth, const size_t renderWindowHeight,
    std::vector<std::unique_ptr<Graphics::Texture> >& textures
)
{
    addMoleculesButtons(controller, windowManager, renderWindowWidth, renderWindowHeight, textures);
    addForcerButtons   (controller, windowManager, renderWindowWidth, renderWindowHeight, textures);
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

void addForcerSprite(
    Graphics::Sprite& forcerSprite, 
    std::vector<std::unique_ptr<Graphics::Texture> >& textures
)
{
    forcerSprite = loadSprite(textures, "media/textures/red.jpeg");
}

void addGraphs(
    Controller& controller, 
    Gui::WindowManager& windowManager, 
    std::vector<std::unique_ptr<Graphics::Font> >& fonts, 
    std::vector<std::unique_ptr<Graphics::Renderable> >& graphicsRenderables,
    const size_t renderWindowWidth, const size_t renderWindowHeight
)
{
    Engine::CoordsSystem temperatureCs{{0.5, 0, 0}, {0, -0.5, 0}, {0, 0, 0.5}, {1, renderWindowHeight, 0}};

    static const double temperatureGraphsWindowHeight = 600;
    static const double temperatureGraphsWindowWidth  = 400;
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
    temperatureGraphTemperature->setPosition(
        {1, renderWindowHeight - temperatureCs.getSizeInPixels(temperatureGraphsWindowHeight)}
    );

    static const size_t fontHeight = 50;
    temperatureGraphTime->setFont(*font);
    temperatureGraphTime->setString("Time");
    temperatureGraphTime->setPosition(
        {1 + temperatureCs.getSizeInPixels(temperatureGraphsWindowWidth), renderWindowHeight - fontHeight}
    );

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
    const size_t renderWindowWidth = renderWindow.getWidth();
    const size_t renderWindowHeight = renderWindow.getHeight();

    addButtons(controller_, windowManager_, renderWindowWidth, renderWindowHeight, textures_);
    addMoleculesSprites(&moleculeSprites_, textures_);
    addForcerSprite(forcerSprite_, textures_);
    addGraphs(controller_, windowManager_, fonts_, graphicsRenderables_, renderWindowWidth, renderWindowHeight);
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

    drawForcer(forcerSprite_, controller_, coordsSystem_, renderWindow_);
}

Gui::WindowManager& View::windowManager() &
{
    return windowManager_;
}

} // namespace Simulator