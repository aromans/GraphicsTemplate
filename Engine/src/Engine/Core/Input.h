#ifndef GRAPHICSTEMPLATE_INPUT_H
#define GRAPHICSTEMPLATE_INPUT_H

#include <algorithm>

namespace Engine {

    class Input {
    public:
        inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

        inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        inline static float GetMouseXPos() { return s_Instance->GetMouseXPosImpl(); }
        inline static float GetMouseYPos() { return s_Instance->GetMouseYPosImpl(); }
    protected:
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXPosImpl() = 0;
        virtual float GetMouseYPosImpl() = 0;
    private:
        static Input* s_Instance;
    };

}

#endif //GRAPHICSTEMPLATE_INPUT_H
