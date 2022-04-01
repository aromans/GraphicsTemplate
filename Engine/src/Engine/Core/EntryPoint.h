#ifndef GRAPHICSTEMPLATE_ENTRYPOINT_H
#define GRAPHICSTEMPLATE_ENTRYPOINT_H

#include <stdio.h>

#include "Application.h"

extern Engine::Application *Engine::CreateApplication();

int main(int argc, char** argv) {
    Engine::Log::Init();
    Engine::Application* game = Engine::CreateApplication();

    // Initialize()
    if (!game->Init()) return 1;

    // Start()
    game->Start();

    float time = 0.0f;
    const float deltaTime = 0.01f;
    float currTime = (float)glfwGetTime();
    float accumulator = 0.0f;

    while (game->IsRunning()) {
        float newTime = (float)glfwGetTime();
        float frameTime = newTime - currTime;
        if (frameTime > 0.25f)
            frameTime = 0.25f;
        currTime = newTime;

        accumulator += frameTime;

        // Input()

        while (accumulator > deltaTime) {
            // Update()
            game->Update(deltaTime);
            time += deltaTime;
            accumulator -= deltaTime;
        }

        // Render()
        game->Render();
    }

    delete game;
    return 0;
}

#endif //GRAPHICSTEMPLATE_ENTRYPOINT_H
