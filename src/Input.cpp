#include "Input.hpp"

GLFWwindow* Input::s_window = nullptr;
std::array<Input::KeyState, GLFW_KEY_LAST + 1> Input::s_keys;

Input::Input(GLFWwindow* glfwWindow)
{
    glfwSetKeyCallback(glfwWindow, Input::KeyCallback);
    std::fill(s_keys.begin(), s_keys.end(), KEY_RELEASED_LONG);
    Input::s_window = glfwWindow;
}

bool Input::GetKeyDown(int key)
{
    return s_keys[key] == KEY_PRESSED_ONCE;
}

bool Input::GetKey(int key)
{
    return s_keys[key] == KEY_PRESSED_ONCE || s_keys[key] == KEY_PRESSED_ONCE;
}

bool Input::GetKeyUp(int key)
{
    return s_keys[key] == KEY_RELEASED_ONCE;
}

// NOTE: call in the end of loop
void Input::Update()
{
    for(int i = 0; i < s_keys.size(); i++)
    {
        if(s_keys[i] == KEY_PRESSED_ONCE)
            s_keys[i] = KEY_PRESSED_LONG;
        else if(s_keys[i] == KEY_RELEASED_ONCE)
            s_keys[i] = KEY_RELEASED_LONG;
    }
}

void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        s_keys[key] = KEY_PRESSED_ONCE;
    }
    else if (action == GLFW_RELEASE)
    {
        s_keys[key] = KEY_RELEASED_ONCE;
    }
}
