#include "Graphics/GraphicsWindow.hpp"
#include "Graphics/Events.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Sprite.hpp"

#include "Model/Molecule.hpp"
#include "Model/MoleculeManager.hpp"

#include "Utils/Rand.hpp"

#include <iostream>

#include "Logger/Log.h"

#include "View/View.hpp"
#include "Controller/Controller.hpp"
#include "View/Gui/GraphsWindow.hpp"

int main(const int argc, const char* argv[])
{
    LogOpen(argv[0]);

    srand(time(NULL));

    Simulator::MoleculeManager moleculeManager({0, 0, 0}, {800, 600, 0}); // TODO 

    auto& molecules  = moleculeManager.molecules();
    auto& boundaries = moleculeManager.boundaries();

    boundaries.push_back(Simulator::Boundary(Engine::Point{0, 0, 0}, 0, 600,   Engine::Vector(1, 0, 0)));
    boundaries.push_back(Simulator::Boundary(Engine::Point{0, 0, 0}, 800, 0,   Engine::Vector(0, 1, 0)));
    boundaries.push_back(Simulator::Boundary(Engine::Point{0, 600, 0}, 800, 0, Engine::Vector(0, -1, 0)));
    boundaries.push_back(Simulator::Boundary(Engine::Point{800, 0, 0}, 0, 600, Engine::Vector(-1, 0, 0)));

    static const double basicMass     = 1;
    static const double basicSpeedAbs = 400;
    static const double basicRadius   = 5;
    static const double basicHeight   = 5;
    static const double basicWidth    = 5;

    Simulator::Molecule::basicMass           (basicMass);
    Simulator::Molecule::basicSpeedAbs       (basicSpeedAbs);
    Simulator::CircleMolecule::basicRadius   (basicRadius);
    Simulator::RectangleMolecule::basicHeight(basicHeight);
    Simulator::RectangleMolecule::basicWidth (basicWidth);

    Simulator::Controller controller{moleculeManager};

    Graphics::RenderWindow renderWindow{800, 600, "molecules"};
    Engine::CoordsSystem coordsSystem{1, {0, 0, 0}}; // TODO: FIX BTN

    Simulator::View view{controller, renderWindow, coordsSystem};

    while (renderWindow.isOpen())
    {
        Graphics::Event event;
        while (renderWindow.pollEvent(event))
        {
            if (event.type == Graphics::Event::EventType::Closed)
                renderWindow.close();
        }

        renderWindow.clear();
    
        moleculeManager.moveMolecules();

        view.update(event);
        view.draw();
        renderWindow.display();
    }
}