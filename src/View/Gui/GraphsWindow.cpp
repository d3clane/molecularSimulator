#include "View/Gui/GraphsWindow.hpp"
#include "Graphics/Circle.hpp"

#include <cmath>
#include <cassert>
#include <iostream>

namespace Simulator
{

namespace 
{

std::chrono::milliseconds getDeltaTime(const std::chrono::steady_clock::time_point& prevTime)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - prevTime
    );
}

void updatePoints(
    const double newValY, std::deque<Engine::Point>& points, const double stepBetweenPoints,
    const double deltaTime, const double timePerPoint
)
{
    points.pop_front();

    double shiftBack = points.front().x;

    for (auto& point : points)
    {
        point.x -= shiftBack;
    }

    const double endPointX = points.back().x + deltaTime / timePerPoint * stepBetweenPoints;

    points.push_back(Engine::Point{endPointX, newValY, 0});
}

} // namespace anon

GraphsWindow::GraphsWindow(
    const Engine::CoordsSystem& coordsSystem, const Engine::Point& topLeft,
    const double width, const double height, 
    const std::chrono::milliseconds& timePerPoint, const double stepBetweenPoints,
    Controller& controller
) : coordsSystem_(coordsSystem), topLeft_(topLeft), width_(width), height_(height),
    timePerPoint_(timePerPoint), stepBetweenPoints_(stepBetweenPoints),
    numberOfPointsPerScreen_(std::ceil(width / stepBetweenPoints_)),
    controller_(controller)
{
    assert(width > 0);
    assert(height > 0);
    assert(timePerPoint.count() > 0);
    assert(stepBetweenPoints > 0);

    Engine::Point point{0, 0, 0};
    for (size_t i = 0; i < numberOfPointsPerScreen_; ++i)
    {
        points_.push_back(point);

        point.x += stepBetweenPoints;
    }

    prevTime_ = std::chrono::steady_clock::now();
}

void GraphsWindow::draw(Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& unusedCs)
{
    static const double radiusAlignCoeff = 4;
    static const unsigned int basicPixelRadius = 2;

    for (const auto& point : points_)
    {
        Graphics::Circle circle{basicPixelRadius};
        Graphics::WindowPoint circlePos = coordsSystem_.getPosInWindow(point);

        std::cout << "POS IN CS: " << point.x << " " << point.y << std::endl;
        circle.setCenterPos(circlePos.x, circlePos.y);

        renderWindow.drawCircle(circle);

        std::cout << "POS: " << circlePos.x << " " << circlePos.y << std::endl;
    }
}

// TODO: Think about copyPaste

bool TemperatureGraphsWindow::update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    std::chrono::milliseconds deltaTime = getDeltaTime(prevTime_);

    if (deltaTime.count() < timePerPoint_.count())
        return true;
    
    const double temperature = controller_.getTemperature();

    updatePoints(temperature, points_, stepBetweenPoints_, deltaTime.count(), timePerPoint_.count());

    prevTime_ = std::chrono::steady_clock::now();

    return true;
}


bool PressureGraphsWindow::update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event)
{
    std::chrono::milliseconds deltaTime = getDeltaTime(prevTime_);

    if (deltaTime.count() < timePerPoint_.count())
        return true;
    
    const double pressure = controller_.getPressure();

    updatePoints(pressure, points_, stepBetweenPoints_, deltaTime.count(), timePerPoint_.count());

    prevTime_ = std::chrono::steady_clock::now();

    return true;
}

} // namespace Simulator