#include "Window.hpp"
#include "Renderer.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "Polygon.hpp"
#include "Physics.hpp"
#include "Player.hpp"
#include "Planet.hpp"
#include "TestPolygon.hpp"

int main()
{
    Window* window = &Window::instance();
    window->init("Astra", 720, 480, true);

    Input* input = &Input::instance();
    input->init(window->glfwWindow);

    Renderer* renderer = &Renderer::instance();
    Image testUV = Image("res/test.png");

    AudioManager* audioManager = &AudioManager::instance();
    Audio audio = Audio("res/game_over.wav");

    Planet planet = Planet(100);
    Player player = Player(Vector(0, 0));

    Physics* physics = &Physics::instance();
    const float PHYSICS_UPDATE_RATE = 120.0f; // Number of physics updates per second
    const float FIXED_TIME_STEP = 1.0f / PHYSICS_UPDATE_RATE; // Time step between physics updates
    float accumulatedTime = 0.0f;

    Font font = Font("res/font24x24.png", 24);

    // Render loop
    while (window->isOpen())
    {
        renderer->fillBackground(0x050505);

        player.update(window->deltaTime);
        
        // Physics loop with fixed time
        accumulatedTime += window->deltaTime;
        while(accumulatedTime >= FIXED_TIME_STEP)
        {
            physics->step(FIXED_TIME_STEP);
            accumulatedTime -= FIXED_TIME_STEP;
        }

        window->xOffset = -player.m_polygon.transform.pos.x;
        window->yOffset = -player.m_polygon.transform.pos.y;

        planet.draw();
        player.draw();
        renderer->drawText("Hi World! \nhow are you?", &font, 0, 0, 0.1f, 0xFFA0FF);

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
        // if(input->getKeyDown(GLFW_KEY_SPACE))
        // {
        //     audio.play(1.0f); // Play sound
        // }

        if(input->isKeyDown(GLFW_KEY_ESCAPE))
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