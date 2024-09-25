#include "Graphics/GraphicsWindow.hpp"
#include "Graphics/Events.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Sprite.hpp"

#include "Scene/Molecule.hpp"
#include "Scene/MoleculeManager.hpp"

#define TEXTURE_LOAD(FILE_NAME, TEXTURE_NAME, SPRITE_NAME)      \ 
    Graphics::Texture TEXTURE_NAME;                             \
    Graphics::Sprite  SPRITE_NAME;                              \
    TEXTURE_NAME.loadFromFile(FILE_NAME);                       \
    SPRITE_NAME.setTexture(TEXTURE_NAME); 

int main()
{
    Scene::MoleculeManager moleculeManager;

    auto& circleMolecules    = moleculeManager.circleMolecules();
    auto& rectangleMolecules = moleculeManager.rectangleMolecules();
    auto& boundaries         = moleculeManager.boundaries();

    TEXTURE_LOAD("media/textures/whiteCircle.png", whiteTexture, whiteSprite);

    boundaries.push_back(Scene::Boundary(Engine::Point{0, 0, 0}, 0, 600,   Engine::Vector(1, 0, 0)));
    boundaries.push_back(Scene::Boundary(Engine::Point{0, 0, 0}, 800, 0,   Engine::Vector(0, 1, 0)));
    boundaries.push_back(Scene::Boundary(Engine::Point{0, 600, 0}, 800, 0, Engine::Vector(0, -1, 0)));
    boundaries.push_back(Scene::Boundary(Engine::Point{800, 0, 0}, 0, 600, Engine::Vector(-1, 0, 0)));

    // TODO: coords system

    const double v = 0.1;

#if 0
        circleMolecules.push_back(
            Scene::CircleMolecule(
                5, Scene::Molecule::CtorParams(
                    whiteSprite, Engine::Point(50, 300, 0), 1, 
                    Engine::Vector(0, v, 0)
                )
            )
        );

        circleMolecules.push_back(
            Scene::CircleMolecule(
                5, Scene::Molecule::CtorParams(
                    whiteSprite, Engine::Point(50, 500, 0), 1, 
                    Engine::Vector(0, -v, 0)
                )
            )
        );
#endif

    for (int i = 0; i < 50; ++i)
    {
        double v_x = (rand() % 100) / 100 * v;
        int dirX = rand() % 2 == 0 ? -1 : 1;
        int dirY = rand() % 2 == 0 ? -1 : 1;

        circleMolecules.push_back(
            Scene::CircleMolecule(
                5, Scene::Molecule::CtorParams(
                    whiteSprite, Engine::Point(rand() % 600 + 50, rand() % 400 + 50, 0), 1, 
                    Engine::Vector(dirX * v_x, dirY * std::sqrt(v * v - v_x * v_x), 0)
                )
            )
        );
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
        
        for (auto& molecule : circleMolecules)
            window.drawSprite(molecule);

        moleculeManager.moveMolecules();

        window.display();
    }


}