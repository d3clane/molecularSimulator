#include "Graphics/GraphicsWindow.hpp"
#include "Graphics/Events.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Sprite.hpp"

#include "Model/Molecule.hpp"
#include "Model/MoleculeManager.hpp"
#include "Model/Forcer.hpp"

#include "Utils/Rand.hpp"

#include <iostream>

#include "Logger/Log.h"

#include "View/View.hpp"
#include "Controller/Controller.hpp"
#include "View/Gui/GraphsWindow.hpp"

#include "Utils/Exceptions.hpp"

void addBoundaries(Simulator::Controller& controller, const size_t width, const size_t height)
{
    static const size_t infiniteSize = 10000;

    controller.addBoundary(Simulator::Boundary(Engine::Point{0, 0, 0}, 0, height,     Engine::Vector(1, 0, 0)));
    controller.addBoundary(Simulator::Boundary(Engine::Point{0, 0, 0}, width, 0,      Engine::Vector(0, 1, 0)));
    controller.addBoundary(Simulator::Boundary(Engine::Point{0, height, 0}, width, 0, Engine::Vector(0, -1, 0)));

    //controller.addBoundary(Simulator::Boundary(Engine::Point{width, 0, 0}, 0, height, Engine::Vector(-1, 0, 0)));
}

int main(const int argc, const char* argv[])
{
    try
    {
        LogOpen(argv[0]);

        srand(time(NULL));

        static const size_t stepInPixelsBasicCs = 1;
        static const Engine::Point basicCsBeginPoint{0, 0, 0};
        Engine::CoordsSystem coordsSystem{stepInPixelsBasicCs, basicCsBeginPoint};

        static const size_t width  = 800;
        static const size_t height = 600;
        
        static const size_t forcerX = 700;
        static const size_t forcerHeight = 600;
        static const size_t forcerWidth = 10000;
        
        Simulator::Forcer forcer{Engine::Point{forcerX, 0, 0}, forcerWidth, height, {-1, 0, 0}};
        Simulator::MoleculeManager moleculeManager({0, 0, 0}, {forcerX, height, 0});
        moleculeManager.forcer(forcer);

        Simulator::Controller controller{moleculeManager};
        addBoundaries(controller, width, height);

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

        Graphics::RenderWindow renderWindow{width, height, "molecules"};

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

            //controller.moveForcerUp(std::chrono::milliseconds(1));
        }
    }
    catch(int e)
    {
        std::cerr << "Error with code " << e << "\n";
    }
    catch(const char* e)
    {
        std::cerr << "Error with message: " << e << "\n";
    }
    catch(const std::string& e)
    {
        std::cerr << "Error with message: " << e << "\n";
    }
    catch(const std::exception* e)
    {
        std::cerr << e->what();
    }
}