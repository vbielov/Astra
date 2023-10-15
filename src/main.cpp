#include "Window.hpp"
#include "OpenGL.hpp"
#include "Audio.hpp"
#include "Input.hpp"

int main()
{
    Window window = Window("Window", 720, 480, true);
    Input input = Input(window.GetGLFW());
    OpenGL openGL = OpenGL();

    Image testUV = Image("res/test.png");
    Image slime = Image("res/slime.png");
    Image button = Image("res/blue_button.png");

    Audio audio = Audio();
    audio.Play("res/game_over.wav");
    
    // Render loop
    int i = 0;
    while (window.IsOpen())
    {
        openGL.FillBackground(0x300a24);
        
        openGL.DrawImage(&slime, 0, 0, 16, 16, -1, -1, 1, 1);
        openGL.DrawImage(&button, 0, 0, 190, 49, -1, 0, 1, 49.0f / 190.0f);
        openGL.DrawImage(&testUV, 0, 0, 4096, 4096, 0, 0, 1, 1);
        openGL.DrawRect((float)i / 100.0f, 0, 0.5f, 0.5f, 0xa5a8e6);
        openGL.DrawRect(-1.0f, -1.0f, 0.5f, 0.5f, 0x327da8);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(200, 60));
        if(ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground))
        {
            ImGui::TextUnformatted(("fps: " + std::to_string(window.fps)).c_str());
            ImGui::TextUnformatted(("window: " + std::to_string(Window::width) + "x" + std::to_string(Window::height)).c_str());
        }
        ImGui::End();

        if(Input::IsKeyPressed(GLFW_KEY_W))
        {
            i++;
        }

        openGL.Render();
        window.EndFrame();
    }

    return 0;
}