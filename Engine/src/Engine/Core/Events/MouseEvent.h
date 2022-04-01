#ifndef GRAPHICSTEMPLATE_MOUSEEVENT_H
#define GRAPHICSTEMPLATE_MOUSEEVENT_H

#include "Event.h"

#include <sstream>

namespace Engine {

    class MouseButtonEvent : public Event {
    public:
        inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CATEGORY_TYPE(EventCategoryMouseButton | EventCategoryInput);
    protected:
        MouseButtonEvent(int keyCode) : m_KeyCode(keyCode) { }

        int m_KeyCode;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int keyCode) : MouseButtonEvent(keyCode) { }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << GetKeyCode();
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonPressed);
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int keyCode) : MouseButtonEvent(keyCode) { }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << GetKeyCode();
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::MouseButtonReleased);
    };

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y) : m_XPos(x), m_YPos(y) { }

        inline std::tuple<float, float> GetMousePos() const {
            return std::make_tuple(m_XPos, m_YPos);
        }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: (" << m_XPos << ", " << m_YPos << ")";
            return ss.str();
        }

        EVENT_CATEGORY_TYPE(EventCategoryMouse | EventCategoryInput);
        EVENT_CLASS_TYPE(EventType::MouseMoved);
    private:
        float m_XPos;
        float m_YPos;
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) { }

        inline std::tuple<float, float> GetMouseScrollOffset() const {
            return std::make_tuple(m_XOffset, m_YOffset);
        }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: (" << m_XOffset << ", " << m_YOffset << ")";
            return ss.str();
        }

        EVENT_CATEGORY_TYPE(EventCategoryMouse | EventCategoryInput);
        EVENT_CLASS_TYPE(EventType::MouseScrolled);
    private:
        float m_XOffset;
        float m_YOffset;
    };

}

#endif //GRAPHICSTEMPLATE_MOUSEEVENT_H
