#ifndef SCENE_VECTOR_HPP
#define SCENE_VECTOR_HPP

#include "Engine/Vector.hpp"
#include "Scene/Renderable.hpp"

namespace Scene
{

struct DrawableVector : public Engine::Vector, public Renderable
{
    Engine::Point beginPos;

    DrawableVector(const Engine::Vector& v, const Engine::Point& beginPos = {0, 0, 0});

    void draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs) override;

    operator Engine::Vector();
};

} // namespace Scene

#endif // SCENE_VECTOR_HPP