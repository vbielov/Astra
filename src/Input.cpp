#include "Input.hpp"

Input &Input::instance()
{
    static Input INSTANCE;
    return INSTANCE;
}

void Input::init(GLFWwindow *glfwWindow)
{
    glfwSetKeyCallback(glfwWindow, Input::keyCallback);
    std::fill(m_keys.begin(), m_keys.end(), KEY_RELEASED_LONG);
    m_window = glfwWindow;
}

bool Input::getKeyDown(int key)
{
    return m_keys[key] == KEY_PRESSED_ONCE;
}

bool Input::getKey(int key)
{
    return m_keys[key] == KEY_PRESSED_ONCE || m_keys[key] == KEY_PRESSED_LONG;
}

bool Input::getKeyUp(int key)
{
    return m_keys[key] == KEY_RELEASED_ONCE;
}

// NOTE: call in the end of game loop
void Input::update()
{
    for(auto& key : m_keys)
    {
        if(key == KEY_PRESSED_ONCE)
        {
            key = KEY_PRESSED_LONG;
            continue;
        }
        if(key == KEY_RELEASED_ONCE)
        {
            key = KEY_RELEASED_LONG;
        }
    }
}

Input::Input()
    : m_window(nullptr), m_keys()
{

}

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Input* input = &Input::instance();
    if (action == GLFW_PRESS)
    {
        input->m_keys[key] = KEY_PRESSED_ONCE;
        return;
    }
    if (action == GLFW_RELEASE)
    {
        input->m_keys[key] = KEY_RELEASED_ONCE;
    }
}
