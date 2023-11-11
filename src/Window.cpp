#include "Window.hpp"

Window& Window::instance()
{
    static Window INSTANCE;
    return INSTANCE;
}

void Window::init(const char* name, int width, int height, bool vsync)
{
    this->width = width;
    this->height = height;
    
    if(!glfwInit())
    {
        std::cout << "Error: GLFW can't init." << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindow = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if (!glfwWindow) {
        std::cout << "Error: Window is not created." << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(glfwWindow);
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
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init();

    glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);

    m_previousTimeFPS = glfwGetTime();
    m_previousTimeDelta = glfwGetTime();
}

Window::Window()
    :   glfwWindow(nullptr), width(0), height(0), fps(0), deltaTime(0.0f), xOffset(0.0f), yOffset(0.0f), scale(1.0f),
        m_frameCounter(0), m_previousTimeDelta(0.0), m_previousTimeFPS(0.0)
{

}

Window::~Window()
{
    if(glfwWindow)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
    }
}

// Don't use it anywhere except of game loop while(isOpen())
bool Window::isOpen()
{
    bool isOpen = glfwWindow && !glfwWindowShouldClose(glfwWindow);
    if(isOpen)
    {
        calculateFPS();
        calculateDeltaTime();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow();
    }
    return isOpen;
}

void Window::endFrame() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

Vector Window::mouseToWorldPosition(Vector cursorPos)
{
    float a = (float)width / (float)height; // aspect ratio
    float x = xOffset * scale;
    float y = yOffset * scale;

    Vector pos = Vector(cursorPos.x / (float)width, 1.0f - cursorPos.y / (float)height) * 2.0f - Vector(1.0f, 1.0f);
    pos.x /= (scale + x) / a;
    pos.y /= scale + y; 

    return pos;
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window* instance = &Window::instance();
    instance->width = width;
    instance->height = height;
}

void Window::calculateFPS()
{
    // Measure speed
    double currentTime = glfwGetTime();
    m_frameCounter++;
    // If a second has passed.
    if ( currentTime - m_previousTimeFPS >= 1.0 )
    {
        // store frame count
        fps = m_frameCounter;

        m_frameCounter = 0;
        // m_previousTime will be assigned in IsOpen()
        m_previousTimeFPS = glfwGetTime();
    }
}

void Window::calculateDeltaTime()
{
    double currentTime = glfwGetTime();
    deltaTime = currentTime - m_previousTimeDelta;
    m_previousTimeDelta = currentTime;
}
