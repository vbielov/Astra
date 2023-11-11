#pragma once
#include <GLFW/glfw3.h>
#include "Vector.hpp"
#include <iostream>
#include <array>

class Input
{
public:
    static Input& instance();
    void init(GLFWwindow* glfwWindow);

    bool isKeyDown(int key);
    bool isKey(int key);
    bool isKeyUp(int key);

    bool isMouseButtonDown(int key);
    bool isMouseButton(int key);
    bool isMouseButtonUp(int key);

    Vector getMousePosition();
    void update();

private:
    GLFWwindow* m_window;

    enum KeyState {
        KEY_RELEASED_LONG = 0,
        KEY_RELEASED_ONCE = 1,
        KEY_PRESSED_LONG = 2,
        KEY_PRESSED_ONCE = 3
    };
    std::array<KeyState, GLFW_KEY_LAST + 1> m_keys;
    std::array<KeyState, GLFW_MOUSE_BUTTON_LAST + 1> m_mouseButtons;

    double m_mousePosX;
    double m_mousePosY;

    Input();
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
};