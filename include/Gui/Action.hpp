#ifndef GUI_ACTION_HPP
#define GUI_ACTION_HPP

#include "Gui/Button.hpp"

namespace Gui
{

class Action
{
public:
    virtual void operator()() = 0;

    virtual ~Action() = default;
};

class ActionWithButton : public Action
{
protected:
    Button* button_;

public:
    ActionWithButton(Button* button) : button_(button) {}

    virtual void operator()() override = 0;
};

class EmptyAction : public Action
{
public:
    virtual void operator()() override { return; }
};

} // namespace Gui


#endif // GUI_ACTION_HPP