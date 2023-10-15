#include "Input.hpp"

GLFWwindow* Input::s_window = nullptr;

Input::Input(GLFWwindow* glfwWindow)
{
    glfwSetKeyCallback(glfwWindow, Input::KeyCallback);
    Input::s_window = glfwWindow;
}

bool Input::IsKeyPressed(int keyCode)
{
    bool isPressed = glfwGetKey(Input::s_window, keyCode) == GLFW_PRESS;
    return isPressed;
}

void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    
}
