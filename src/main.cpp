#include "Window.hpp"
#include "Renderer.hpp"
#include "Audio.hpp"
#include "Input.hpp"

int main()
{
    Window* window = &Window::instance();
    window->init("Astra", 720, 480, false);

    Input* input = &Input::instance();
    input->init(window->glfwWindow);

    Renderer* renderer = &Renderer::instance();
    Image testUV = Image("res/test.png");

    AudioManager* audioManager = &AudioManager::instance();
    Audio audio = Audio("res/game_over.wav");

    // Render loop
    while (window->isOpen())
    {
        renderer->fillBackground(0x300A24);

        renderer->drawRect(-1.0f, -1.0f, 2.0f, 2.0f, 0x000020);
        renderer->drawImage(&testUV, 0, 0, 4096, 4096, -0.5f, -0.5f, 1, 1);

        // Debug with ImGui
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(200, 80));
        if(ImGui::Begin("Debug", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground))
        {
            ImGui::TextUnformatted(("window: " + std::to_string(window->width) + "x" + std::to_string(window->height)).c_str());
            ImGui::TextUnformatted(("fps: " + std::to_string(window->fps)).c_str());
            ImGui::TextUnformatted(("sounds: " + std::to_string(audioManager->sources.size())).c_str());
        }
        ImGui::End();

        // Check input
        if(input->getKeyDown(GLFW_KEY_SPACE))
        {
            audio.play(1.0f); // Play sound
        }

        if(input->getKeyDown(GLFW_KEY_ESCAPE))
        {
            return 0;
        }

        audioManager->update();
        input->update();
        renderer->render();
        window->endFrame();
    }

    return 0;
}