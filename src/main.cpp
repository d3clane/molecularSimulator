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

void addBoundaries(Simulator::MoleculeManager& moleculeManager)
{
    static const size_t infiniteSize = 10000;

    auto& boundaries = moleculeManager.boundaries();

    auto topLeftPoint     = moleculeManager.boundaryTopLeft();
    auto bottomRightPoint = moleculeManager.boundaryBottomRight();
    size_t width  = bottomRightPoint.x - topLeftPoint.x;
    size_t height = bottomRightPoint.y - topLeftPoint.y;

    boundaries.push_back(
        Simulator::Boundary(topLeftPoint, 0, height, Engine::Vector(1, 0, 0))
    );
    boundaries.push_back(
        Simulator::Boundary(topLeftPoint, width, 0, Engine::Vector(0, 1, 0))
    );
    boundaries.push_back(
        Simulator::Boundary(
            Engine::Point{topLeftPoint.x, bottomRightPoint.y, 0}, 
            width, infiniteSize, Engine::Vector(0, -1, 0))
    );
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

        static const size_t width  = 1500;
        static const size_t height = 1000;
        
        
        static const size_t moleculeManagerWidth  = 1000;
        static const size_t moleculeManagerHeight = 600;

        static const size_t infiniteWidth = 10000;
        static const size_t forcerRealWidth = 10;
        static const size_t forcerX = moleculeManagerWidth - forcerRealWidth;

        Simulator::Forcer forcer{Engine::Point{forcerX, 0, 0}, infiniteWidth, moleculeManagerHeight, {-1, 0, 0}};
        Simulator::MoleculeManager moleculeManager({0, 0, 0}, {moleculeManagerWidth, moleculeManagerHeight, 0});
        moleculeManager.forcer(forcer);

        Simulator::Controller controller{moleculeManager};
        addBoundaries(moleculeManager);

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