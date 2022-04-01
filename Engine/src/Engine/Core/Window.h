#ifndef GRAPHICSTEMPLATE_WINDOW_H
#define GRAPHICSTEMPLATE_WINDOW_H

#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Core/Events/Event.h"

namespace Engine {

    struct WindowProps {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Game",
                    unsigned int width = 600,
                    unsigned int height = 400)
                    : Title(title), Width(width), Height(height) { }
    };

    class Window {
    public:
        using EventCallBackFn = std::function<void(Event& e)>;

        virtual ~Window() { }

        virtual void Update() = 0;

        virtual glm::mat4 CalcProjMatrix(float FOV, float Z_NEAR, float Z_FAR) = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallBackFn& event) = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };

}

#endif //GRAPHICSTEMPLATE_WINDOW_H
