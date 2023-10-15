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
    static int width;
    static int height;
    int fps;

    Window(const char* name, int width, int height, bool vsync = true);
    ~Window();
    bool IsOpen();
    GLFWwindow* GetGLFW();
    void EndFrame() const;

private:
    GLFWwindow* m_window;
    int m_frameCounter;
    double m_previousTime;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void CalculateFPS();
};