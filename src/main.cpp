#include "Graphics/GraphicsWindow.hpp"
#include "Graphics/Events.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Sprite.hpp"

#include "Model/Molecule.hpp"
#include "Model/MoleculeManager.hpp"

#include <iostream>

#include "src/Loggers/Log.h"

#define TEXTURE_LOAD(FILE_NAME, TEXTURE_NAME, SPRITE_NAME)      \ 
    Graphics::Texture TEXTURE_NAME;                             \
    Graphics::Sprite  SPRITE_NAME;                              \
    TEXTURE_NAME.loadFromFile(FILE_NAME);                       \
    SPRITE_NAME.setTexture(TEXTURE_NAME); 

int main(const int argc, const char* argv[])
{
    LogOpen(argv[0]);

    //srand(time(NULL));

    Model::MoleculeManager moleculeManager;

    auto& molecules  = moleculeManager.molecules();
    auto& boundaries = moleculeManager.boundaries();

    TEXTURE_LOAD("media/textures/whiteCircle.png", whiteTexture, whiteSprite);
    TEXTURE_LOAD("media/textures/red.jpeg", redTexture, redSprite);

    boundaries.push_back(Model::Boundary(Engine::Point{0, 0, 0}, 0, 600,   Engine::Vector(1, 0, 0)));
    boundaries.push_back(Model::Boundary(Engine::Point{0, 0, 0}, 800, 0,   Engine::Vector(0, 1, 0)));
    boundaries.push_back(Model::Boundary(Engine::Point{0, 600, 0}, 800, 0, Engine::Vector(0, -1, 0)));
    boundaries.push_back(Model::Boundary(Engine::Point{800, 0, 0}, 0, 600, Engine::Vector(-1, 0, 0)));

    // TODO: coords system

    const double v = 0.1;

#if 0
    molecules.push_back(
        std::unique_ptr<Model::Molecule>(new Model::CircleMolecule(
            5, 5, Model::Molecule::CtorParams(
                Engine::Point(50, 300, 0), 1, 
                Engine::Vector(0, v, 0)
            ))
        )
    );

    molecules.push_back(
        std::unique_ptr<Model::Molecule>(new Model::CircleMolecule(
            5, 5, Model::Molecule::CtorParams(
                Engine::Point(50, 500, 0), 1, 
                Engine::Vector(0, -v, 0)
            ))
        )
    );
#endif

    for (int i = 0; i < 5; ++i)
    {
        double v_x = (rand() % 100) / 100. * v;
        double v_y = v * v - v_x * v_x;
        v_y = v_y < 0 ? 0 : std::sqrt(v_y);

        int dirX = rand() % 2 == 0 ? -1 : 1;
        int dirY = rand() % 2 == 0 ? -1 : 1;

        Model::CircleMolecule* tmp = new Model::CircleMolecule{
                10, Model::Molecule::CtorParams{
                Engine::Point(rand() % 600 + 50, rand() % 400 + 50, 0), 1,
                Engine::Vector(dirX * v_x, dirY * v_y, 0)
            }
        };

        molecules.push_back(std::unique_ptr<Model::Molecule>(tmp));
    }

    Graphics::RenderWindow window{800, 600, "molecules"};

    while (window.isOpen())
    {
        Graphics::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Graphics::Event::EventType::Closed)
                window.close();
        }

        window.clear();

        for (auto& molecule : molecules)
        {
            Engine::Point topLeft = molecule.get()->topLeft();

            Model::CircleMolecule* tmp = dynamic_cast<Model::CircleMolecule*>(molecule.get());
            
            if (tmp != nullptr)
            {
                whiteSprite.setPosition({topLeft.x, topLeft.y});
                whiteSprite.scaleInPixels({tmp->radius() * tmp->mass(), tmp->radius() * tmp->mass()});
                
                window.drawSprite(whiteSprite);
            }
            else
            {
                Model::RectangleMolecule* tmp = dynamic_cast<Model::RectangleMolecule*>(molecule.get());

                redSprite.setPosition({topLeft.x, topLeft.y});
                redSprite.scaleInPixels({tmp->width() * tmp->mass(), tmp->height() * tmp->mass()});
                
                window.drawSprite(redSprite);

            }

        }

        moleculeManager.moveMolecules();
        window.display();
    }
}