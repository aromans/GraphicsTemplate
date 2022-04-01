#include "Application.h"

#include "Engine/Core/Logger/Log.h"
#include "Engine/Core/Events/Event.h"

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EcsTypes.h"

#include "Engine/Core/Input.h"

#include <functional>

namespace Engine {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        ENG_CORE_INFO("Hello, World! This is the application!\n");
    }

    Application::~Application() {
        m_Window.release();
    }

    bool Application::Init() {
        m_IsRunning = true;

        m_NativeWindow = (GLFWwindow *) m_Window->GetNativeWindow();

        return true;
    }

    void Application::Start() {
    }

    void Application::Update(float dt) {
        ecs.RunSystems(0, dt);
    }

    void Application::Render() {
        m_Window->Update();
    }

    void Application::OnEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_IsRunning = false;
        return false;
    }

    bool Application::OnWindowResize(WindowResizeEvent &e) {
        ENG_CORE_INFO(e);
        return false;
    }

}