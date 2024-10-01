#include "Scene/Vector.hpp"

namespace Scene
{

DrawableVector::DrawableVector(const Engine::Vector& v, const Engine::Point& beginPos) : 
    Engine::Vector(v), beginPos(beginPos) 
{
}

DrawableVector::operator Engine::Vector() { return Engine::Vector(dx, dy, dz); }

void DrawableVector::draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs)
{
    Engine::Point endPos = beginPos + Engine::Vector(dx, dy, dz);

    Graphics::WindowPoint beginPosInWindow = cs.getPointInWindow(beginPos);
    Graphics::WindowPoint endPosInWindow   = cs.getPointInWindow(endPos);

    Graphics::WindowLine mainLine{beginPosInWindow, endPosInWindow};

    renderWindow.drawLine(mainLine);

    Vector normal = getPerpendicular();

    const double prettyLength = 0.05 * length();
    normal.length(prettyLength);

    Vector coDirectionalVector = *this;
    coDirectionalVector.length(prettyLength);

    Vector arrowVector1 =  normal - coDirectionalVector;
    Vector arrowVector2 = -normal - coDirectionalVector;

    Engine::Point endArrowVector1 = endPos + arrowVector1;
    Engine::Point endArrowVector2 = endPos + arrowVector2;

    Graphics::WindowPoint endArrowVector1InWindow = cs.getPointInWindow(endArrowVector1);
    Graphics::WindowPoint endArrowVector2InWindow = cs.getPointInWindow(endArrowVector2);

    renderWindow.drawLine({endPosInWindow, endArrowVector1InWindow});
    renderWindow.drawLine({endPosInWindow, endArrowVector2InWindow});
}

} // namespace Scene
