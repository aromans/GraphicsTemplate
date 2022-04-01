#include <Engine.h>

#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Component.h>
#include <Engine/ECS/ECS.h>

#include <Engine/Core/Input.h>

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Randomness {
    float r;
};

class PhysicsSystem {
public:
    static void Update(const float elapsedMilliseconds,
                       const std::vector<Engine::EntityID>& entities,
                       Position* p,
                       Velocity* v) {

        if (Engine::Input::IsKeyPressed(GLFW_KEY_W)) {
            for(std::size_t i = 0; i < entities.size(); ++i)
            {
                std::cout   << i << " PV -----------------\n"
                            << "P: (" << p[i].x << ',' << p[i].y << ")\n"
                            << std::endl;

                p[i].y += v[i].y * (elapsedMilliseconds / 1000.f);
            }
        }
    }
};

class Sandbox : public Engine::Application {
public:

    Sandbox() {
        ENG_INFO("This is from the Game itself!\n");

        ecs.RegisterComponent<Position>();
        ecs.RegisterComponent<Velocity>();
        ecs.RegisterComponent<Randomness>();

        Engine::Entity player(ecs);
        player.Add<Position>({0, 0});
        player.Add<Velocity>({0.5f, 0.5f});
        player.Add<Randomness>({0.25f});

        Engine::Entity barrier(ecs);
        barrier.Add<Position>({1, 1});
        player.Add<Randomness>({0.8f});

        movementSystem = new Engine::System<Position, Velocity>(ecs, 0);
        movementSystem->Action(PhysicsSystem::Update);
    }

    ~Sandbox() {
        delete movementSystem;
    }

private:
    Engine::System<Position, Velocity>* movementSystem;
};

Engine::Application* Engine::CreateApplication() {
    return new Sandbox();
}