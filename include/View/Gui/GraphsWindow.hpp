#ifndef VIEW_GUI_GRAPHS_WINDOW_HPP
#define VIEW_GUI_GRAPHS_WINDOW_HPP

#include "Gui/Window.hpp"
#include "Engine/CoordsSystem.hpp"
#include "Controller/Controller.hpp"

#include <deque>


namespace Simulator
{

class GraphsWindow : public Gui::Window
{
protected:
    Engine::CoordsSystem coordsSystem_;

    Engine::Point topLeft_;
    const double width_, height_;

    std::deque<Engine::Point> points_;

    double stepBetweenPoints_;
    size_t numberOfPointsPerScreen_;

    std::chrono::milliseconds timePerPoint_;
    std::chrono::steady_clock::time_point prevTime_;

    Controller& controller_;

public:
    GraphsWindow(
        const Engine::CoordsSystem& coordsSystem, const Engine::Point& topLeft,
        const double width, const double height, 
        const std::chrono::milliseconds& timePerPoint, const double stepBetweenPoints,
        Controller& controller
    );

    void draw  (Graphics::RenderWindow& renderWindow, const Engine::CoordsSystem& cs) override;
    bool update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event  ) = 0;

private:
    void drawGrid(Graphics::RenderWindow& renderWindow);
};

class TemperatureGraphsWindow : public GraphsWindow
{
public:
    TemperatureGraphsWindow(
        const Engine::CoordsSystem& coordsSystem, const Engine::Point& topLeft,
        const double width, const double height, 
        const std::chrono::milliseconds& timePerPoint, const double stepBetweenPoints,
        Controller& controller
    ) : GraphsWindow(
        coordsSystem, topLeft, width, height, timePerPoint, stepBetweenPoints, controller
    ) {}

    bool update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
};

class PressureGraphsWindow : public GraphsWindow
{
public:
    PressureGraphsWindow(
        const Engine::CoordsSystem& coordsSystem, const Engine::Point& topLeft,
        const double width, const double height, 
        const std::chrono::milliseconds& timePerPoint, const double stepBetweenPoints,
        Controller& controller
    ) : GraphsWindow(
        coordsSystem, topLeft, width, height, timePerPoint, stepBetweenPoints, controller
    ) {}

    bool update(Graphics::RenderWindow& renderWindow, const Graphics::Event& event) override;
};

} // namespace Simulator

#endif // VIEW_GUI_GRAPHS_WINDOW_HPP