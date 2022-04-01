#ifndef GRAPHICSTEMPLATE_WINDOWMANAGER_H
#define GRAPHICSTEMPLATE_WINDOWMANAGER_H


//#define GLFW_INCLUDE_NONE 1
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine/Core/Window.h"

namespace Engine {

    class GLWindow : public Window {
    public:
        GLWindow(const WindowProps& props);
        virtual ~GLWindow();

        void Update() override;

        glm::mat4 CalcProjMatrix(float FOV, float Z_NEAR, float Z_FAR) override;

        inline void SetEventCallback(const EventCallBackFn& callback) override { m_Data.callBackFn = callback; }

        unsigned int GetWidth() const override { return m_Data.Width; }
        unsigned int GetHeight() const override { return m_Data.Height; }

        void * GetNativeWindow() const override { return m_Window; }

    private:
        void Init(const WindowProps& data);
        void ShutDown();

    private:
        GLFWwindow* m_Window;

        struct WindowData {
            std::string Title;
            unsigned int Width;
            unsigned int Height;

            EventCallBackFn callBackFn;
        };

        WindowData m_Data;
    };

}

#endif //GRAPHICSTEMPLATE_WINDOWMANAGER_H
