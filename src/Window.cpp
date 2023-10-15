#include "Window.hpp"

int Window::width = 0;
int Window::height = 0;

Window::Window(const char *name, int width, int height, bool vsync)
    :   fps(0), m_window(nullptr), 
        m_frameCounter(0), m_previousTime(0.0)
{
    Window::width = width;
    Window::height = height;

    if(!glfwInit())
    {
        std::cout << "Error: GLFW can't init." << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if (!m_window) {
        std::cout << "Error: Window is not created." << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(vsync);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Error: Glew is not okay." << std::endl;
        glfwTerminate();
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();

    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

    m_previousTime = glfwGetTime();
}

Window::~Window()
{
    if(m_window)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
    }
}

bool Window::IsOpen()
{
    bool isOpen = m_window && !glfwWindowShouldClose(m_window);
    if(isOpen)
    {
        CalculateFPS();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow();
    }
    return isOpen;
}

GLFWwindow* Window::GetGLFW()
{
    return m_window;
}

void Window::EndFrame() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;
}

void Window::CalculateFPS()
{
    // Measure speed
    double currentTime = glfwGetTime();
    m_frameCounter++;
    // If a second has passed.
    if ( currentTime - m_previousTime >= 1.0 )
    {
        // store frame count
        fps = m_frameCounter;

        m_frameCounter = 0;
        m_previousTime = currentTime;
    }
}
