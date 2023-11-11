#include "Window.hpp"
#include "Renderer.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "Polygon.hpp"
#include "Physics.hpp"
#include "Player.hpp"
#include "Terrain.hpp"
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


    Vector points2[] = {
        Vector(-1, -1),
        Vector(-1, -0.8f),
        Vector(1, -0.8f),
        Vector(1, -1)
    };
    Polygon groundPoly = Polygon(points2, 4, false);

    // Player player = Player(Vector(0, 0));
    // Terrain terrain = Terrain();
    TestPolygon test = TestPolygon();
    TestPolygon test2 = TestPolygon();

    Physics* physics = &Physics::instance();
    physics->addPolygon(&groundPoly);
    physics->addPolygon(&test.polygon);
    physics->addPolygon(&test2.polygon);

    const float PHYSICS_UPDATE_RATE = 120.0f; // Number of physics updates per second
    const float FIXED_TIME_STEP = 1.0f / PHYSICS_UPDATE_RATE; // Time step between physics updates
    float accumulatedTime = 0.0f;

    // Render loop
    while (window->isOpen())
    {
        renderer->fillBackground(0x300A24);

        // player.update(window->deltaTime);
        
        // Physics loop with fixed time

        test.update(window->deltaTime);
        test2.update(window->deltaTime);

        accumulatedTime += window->deltaTime;
        while(accumulatedTime >= FIXED_TIME_STEP)
        {
            physics->step(FIXED_TIME_STEP);
            accumulatedTime -= FIXED_TIME_STEP;
        }

        // window->xOffset = -player.m_polygon.transform.pos.x;
        // window->yOffset = -player.m_polygon.transform.pos.y;

        // player.m_polygon.draw(0xFF00FF);
        // player.draw();
        // terrain.draw();

        test.draw(0xFFFFFF);
        test2.draw(0xFFFFFF);

        groundPoly.draw(0x00FF00);

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