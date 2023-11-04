#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>

class Input
{
public:
    static Input& instance();
    void init(GLFWwindow* glfwWindow);
    bool getKeyDown(int key);
    bool getKey(int key);
    bool getKeyUp(int key);
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

    Input();
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};