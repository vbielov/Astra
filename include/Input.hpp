#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Input
{
public:
    Input(GLFWwindow* glfwWindow);
    static bool IsKeyPressed(int keyCode);

private:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static GLFWwindow* s_window;
};