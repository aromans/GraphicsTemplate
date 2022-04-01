#ifndef GRAPHICSTEMPLATE_APPLICATION_H
#define GRAPHICSTEMPLATE_APPLICATION_H

#include <memory>
#include <GLFW/glfw3.h>

#include "Events/ApplicationEvent.h"
#include "Engine/ECS/ECS.h"

#include "Window.h"

namespace Engine {

    class Application {
    public:
        Application();
        ~Application();

        bool Init();
        void Start();
        void Update(float dt);
        void Render();

        void OnEvent(Event& e);
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        bool IsRunning() const { return m_IsRunning; }

        inline Window& GetWindow() const { return *m_Window; }
        inline static Application& Get() { return *s_Instance; }

        ECS ecs;
    private:
        GLFWwindow* m_NativeWindow;
        std::unique_ptr<Window> m_Window;
        bool m_IsRunning = true;

        static Application* s_Instance;
    };

    // Defined in client
    Application* CreateApplication();
}

#endif //GRAPHICSTEMPLATE_APPLICATION_H
