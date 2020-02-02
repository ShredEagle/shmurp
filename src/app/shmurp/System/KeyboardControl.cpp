#include "KeyboardControl.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace ad {

void KeyboardControl::addedToEngine(aunteater::Engine &aEngine)
{}
void KeyboardControl::update(double time)
{}

void KeyboardControl::Callback::operator()(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_UP)
    {
        std::cout << "Upping" << std::endl;
    }
}

} // namespace ad
