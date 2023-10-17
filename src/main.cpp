#include "Window.hpp"
#include "OpenGL.hpp"
#include "Audio.hpp"
#include "Input.hpp"

int main()
{
    Window window = Window("Window", 720, 480, true);

    Input input = Input(window.GetGLFW());
    OpenGL openGL = OpenGL();
    Audio audio = Audio("res/game_over.wav");
    Image testUV = Image("res/test.png");

    // Render loop
    while (window.IsOpen())
    {
        openGL.FillBackground(0x300a24);
        openGL.DrawImage(&testUV, 0, 0, 4096, 4096, -0.5f, -0.5f, 1, 1);

        // Debug with ImGui
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(200, 80));
        if(ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground))
        {
            ImGui::TextUnformatted(("window: " + std::to_string(Window::width) + "x" + std::to_string(Window::height)).c_str());
            ImGui::TextUnformatted(("fps: " + std::to_string(window.fps)).c_str());
            ImGui::TextUnformatted(("sounds: " + std::to_string(audio.s_sources.size())).c_str());
        }
        ImGui::End();

        // Check input
        if(Input::GetKeyDown(GLFW_KEY_SPACE))
            audio.Play(1.0f); // Play sound

        if(Input::GetKeyDown(GLFW_KEY_ESCAPE))
            return 0;



        Audio::Update();
        Input::Update();
        openGL.Render();
        window.EndFrame();
    }

    return 0;
}