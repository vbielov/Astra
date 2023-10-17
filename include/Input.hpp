#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>

class Input
{
public:
    Input(GLFWwindow* glfwWindow);
    static bool GetKeyDown(int key);
    static bool GetKey(int key);
    static bool GetKeyUp(int key);
    static void Update();

private:
    enum KeyState {
        KEY_RELEASED_LONG = 0,
        KEY_RELEASED_ONCE = 1,
        KEY_PRESSED_LONG = 2,
        KEY_PRESSED_ONCE = 3
    };

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static GLFWwindow* s_window;
    static std::array<KeyState, GLFW_KEY_LAST + 1> s_keys;
};