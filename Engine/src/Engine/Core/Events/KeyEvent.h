#ifndef GRAPHICSTEMPLATE_KEYBOARDEVENT_H
#define GRAPHICSTEMPLATE_KEYBOARDEVENT_H

#include "Event.h"

#include <sstream>

namespace Engine {

    class KeyEvent : public Event {
    public:
        inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CATEGORY_TYPE(EventCategoryInput | EventCategoryKeyboard);
    protected:
        KeyEvent(int keyCode) : m_KeyCode(keyCode) { }

        int m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keyCode, int repeatCount)
            : KeyEvent(keyCode), m_RepeatCount(repeatCount) { }

        inline int GetRepeatCount() const { return m_RepeatCount; }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << GetKeyCode() << " (RepeatedCount: " << m_RepeatCount << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyPressed)
    protected:
        int m_RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) { }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << GetKeyCode();
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyReleased);
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(int keyCode) : KeyEvent(keyCode) { }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << GetKeyCode();
            return ss.str();
        }

        EVENT_CLASS_TYPE(EventType::KeyTyped);
    };

}

#endif //GRAPHICSTEMPLATE_KEYBOARDEVENT_H
