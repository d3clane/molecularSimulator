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

int main(const int argc, const char* argv[])
{
    LogOpen(argv[0]);

    srand(time(NULL));

    Simulator::MoleculeManager moleculeManager({0, 0, 0}, {800, 600, 0}); // TODO 

    auto& molecules  = moleculeManager.molecules();
    auto& boundaries = moleculeManager.boundaries();

    const double bigWidth = 10000;

    boundaries.push_back(Simulator::Boundary(Engine::Point{-bigWidth, 0, 0}, bigWidth, 600,   Engine::Vector(1, 0, 0)));
    boundaries.push_back(Simulator::Boundary(Engine::Point{0, -bigWidth, 0}, 800, bigWidth,   Engine::Vector(0, 1, 0)));
    boundaries.push_back(Simulator::Boundary(Engine::Point{0, 600, 0}, 800, bigWidth, Engine::Vector(0, -1, 0)));
    boundaries.push_back(Simulator::Boundary(Engine::Point{800, 0, 0}, bigWidth, 600, Engine::Vector(-1, 0, 0)));

    static const double basicMass     = 1;
    static const double basicSpeedAbs = 0.05;
    static const double basicRadius   = 5;
    static const double basicHeight   = 5;
    static const double basicWidth    = 5;

    Simulator::Molecule::basicMass           (basicMass);
    Simulator::Molecule::basicSpeedAbs       (basicSpeedAbs);
    Simulator::CircleMolecule::basicRadius   (basicRadius);
    Simulator::RectangleMolecule::basicHeight(basicHeight);
    Simulator::RectangleMolecule::basicWidth (basicWidth);

    for (int i = 0; i < 20; ++i)
    {
        double vX = Utils::Rand(0, 1) * basicSpeedAbs;
        double vY = basicSpeedAbs * basicSpeedAbs - vX * vX;
        vY = vY < 0 ? 0 : std::sqrt(vY);

        int dirX = rand() % 2 == 0 ? -1 : 1;
        int dirY = rand() % 2 == 0 ? -1 : 1;

        Simulator::CircleMolecule* tmp = new Simulator::CircleMolecule{
            basicRadius, Simulator::Molecule::CtorParams{
                Engine::Point(rand() % 600 + 50, rand() % 400 + 50, 0), basicMass,
                Engine::Vector(dirX * vX, dirY * vY, 0)
            }
        };

        molecules.push_back(std::unique_ptr<Simulator::Molecule>(tmp));
    }

    Graphics::RenderWindow renderWindow{800, 600, "molecules"};
    Engine::CoordsSystem coordsSystem{1, {0, 0, 0}};

    Simulator::Controller controller{moleculeManager};

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
    
#if 0

        for (auto& molecule : molecules)
        {
            Engine::Point topLeft = molecule.get()->topLeft();

            Model::CircleMolecule* tmp = dynamic_cast<Model::CircleMolecule*>(molecule.get());
            
            if (tmp != nullptr)
            {
                whiteSprite.setPosition({topLeft.x, topLeft.y});
                whiteSprite.scaleInPixels({tmp->radius(), tmp->radius()});
                
                renderWindow.drawSprite(whiteSprite);
            }
            else
            {
                Model::RectangleMolecule* tmp = dynamic_cast<Model::RectangleMolecule*>(molecule.get());

                redSprite.setPosition({topLeft.x, topLeft.y});
                redSprite.scaleInPixels({tmp->width(), tmp->height()});
                
                renderWindow.drawSprite(redSprite);
            }

        }
#endif

        moleculeManager.moveMolecules();

        view.update(event);
        view.draw();
        renderWindow.display();
    }
}