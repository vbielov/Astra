#include "Input.hpp"

Input& Input::instance()
{
    static Input INSTANCE;
    return INSTANCE;
}

void Input::init(GLFWwindow *glfwWindow)
{
    glfwSetKeyCallback(glfwWindow, Input::keyCallback);
    glfwSetMouseButtonCallback(glfwWindow, Input::mouseCallback);
    std::fill(m_keys.begin(), m_keys.end(), KEY_RELEASED_LONG);
    m_window = glfwWindow;
}

bool Input::isKeyDown(int key)
{
    return m_keys[key] == KEY_PRESSED_ONCE;
}

bool Input::isKey(int key)
{
    return m_keys[key] == KEY_PRESSED_ONCE || m_keys[key] == KEY_PRESSED_LONG;
}

bool Input::isKeyUp(int key)
{
    return m_keys[key] == KEY_RELEASED_ONCE;
}

bool Input::isMouseButtonDown(int key)
{
    return m_mouseButtons[key] == KEY_PRESSED_ONCE;
}

bool Input::isMouseButton(int key)
{
    return m_mouseButtons[key] == KEY_PRESSED_ONCE || m_mouseButtons[key] == KEY_PRESSED_LONG;
}

bool Input::isMouseButtonUp(int key)
{
    return m_mouseButtons[key] == KEY_RELEASED_ONCE;
}

Vector Input::getMousePosition()
{
    return Vector(m_mousePosX, m_mousePosY);
}

// NOTE: call in the end of game loop
void Input::update()
{
    glfwGetCursorPos(m_window, &m_mousePosX, &m_mousePosY);

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

    for(auto& mouseButton : m_mouseButtons)
    {
        if(mouseButton == KEY_PRESSED_ONCE)
        {
            mouseButton = KEY_PRESSED_LONG;
            continue;
        }
        if(mouseButton == KEY_RELEASED_ONCE)
        {
            mouseButton = KEY_RELEASED_LONG;
        }
    }
}

Input::Input()
    : m_window(nullptr), m_keys(), m_mouseButtons(), m_mousePosX(0), m_mousePosY(0)
{

}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void Input::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    Input* input = &Input::instance();
    if(action == GLFW_PRESS)
    {
        input->m_mouseButtons[button] = KEY_PRESSED_ONCE;
        return;
    }
    if (action == GLFW_RELEASE)
    {
        input->m_mouseButtons[button] = KEY_RELEASED_ONCE;
    }
}
