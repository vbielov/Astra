#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui-1.89.8/imgui.h"
#include "imgui-1.89.8/backends/imgui_impl_glfw.h"
#include "imgui-1.89.8/backends/imgui_impl_opengl3.h"

#include <iostream>

class Window
{
public:
    GLFWwindow* glfwWindow;
    int width;
    int height;
    int fps;
    float deltaTime;

    float xOffset;
    float yOffset;
    float scale;
    
    static Window& instance();
    void init(const char* name, int width, int height, bool vsync = true);
    bool isOpen();
    void endFrame() const;

private:
    int m_frameCounter;
    double m_previousTimeDelta;
    double m_previousTimeFPS;

    Window();
    ~Window();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void calculateFPS();
    void calculateDeltaTime();
};