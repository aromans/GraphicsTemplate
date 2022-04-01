#ifndef GRAPHICSTEMPLATE_WINDOWINPUT_H
#define GRAPHICSTEMPLATE_WINDOWINPUT_H

#include "Engine/Core/Input.h"

namespace Engine {

    class GLInput : public Input {
    protected:
        virtual bool IsKeyPressedImpl(int keycode) override;
        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
        virtual float GetMouseXPosImpl() override;
        virtual float GetMouseYPosImpl() override;
    };

}

#endif //DIMENSIONS_WINDOWINPUT_H
