#ifndef SCENE_MOLECULE_HPP
#define SCENE_MOLECULE_HPP

#include "Graphics/Sprite.hpp"

#include "Engine/Collider.hpp"
#include "Engine/Transformable.hpp"
#include "Engine/CoordsSystem.hpp"
#include "Engine/Vector.hpp"

namespace Scene
{

using Engine::Point;
using Engine::Vector;

class Molecule : public Engine::Transformable
{
protected:
    Graphics::Sprite sprite_;

    unsigned int mass_;
    Vector speed_;

public:
    struct CtorParams
    {
        const Graphics::Sprite& sprite;
        const Point& topLeft;
        const unsigned int mass;
        const Vector& speed;

        CtorParams(
            const Graphics::Sprite& sprite, const Point& topLeft, const unsigned int mass, const Vector& speed
        ) : sprite(sprite), topLeft(topLeft), mass(mass), speed(speed) {}
    };

    Molecule(
        const Graphics::Sprite& sprite, const Point& topLeft, 
        const unsigned int mass, const Vector& speed
    );
    Molecule(const CtorParams& ctorParams);

    unsigned int mass() const;
    void         mass(const unsigned int mass);

    const Vector& speed() const &;
    void          speed(const Vector& speed);

    Point topLeft() const;

    operator Graphics::Sprite();
};

class CircleMolecule : public Molecule
{
    double radius_;

    Engine::CircleCollider collider_;

public:
    CircleMolecule(const double radius, const Molecule::CtorParams& ctorParams);
    CircleMolecule(const CircleMolecule&  other);
    CircleMolecule(CircleMolecule&& other);

    Engine::CircleCollider& collider() &;
    double radius() const;

    operator Graphics::Sprite();
};

class RectangleMolecule : public Molecule
{
    double width_, height_;

    Engine::RectangleCollider collider_;

public:
    RectangleMolecule(const double width, const double height, const Molecule::CtorParams& ctorParams);
    RectangleMolecule(const RectangleMolecule& other);

    Engine::RectangleCollider& collider() &;

    double width()  const;
    double height() const;

    operator Graphics::Sprite();
};

} // namespace Scene

#endif // SCENE_MOLECULE_HPP