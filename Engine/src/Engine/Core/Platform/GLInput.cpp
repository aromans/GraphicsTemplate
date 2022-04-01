#include "GLInput.h"

#include <GLFW/glfw3.h>
#include "Engine/Core/Application.h"

namespace Engine {

    Input* Input::s_Instance = new GLInput();

    bool GLInput::IsKeyPressedImpl(int keycode) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool GLInput::IsMouseButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> GLInput::GetMousePositionImpl() {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { (float) xPos, (float) yPos };
    }

    float GLInput::GetMouseXPosImpl() {
        auto [x,y] = GetMousePositionImpl();
        return x;
    }

    float GLInput::GetMouseYPosImpl() {
        auto [x,y] = GetMousePositionImpl();
        return y;
    }

}