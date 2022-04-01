#ifndef GRAPHICSTEMPLATE_EVENT_H
#define GRAPHICSTEMPLATE_EVENT_H

#include <functional>
#include <string>

namespace Engine {

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; } \
                                virtual EventType GetEventType() const override { return GetStaticType(); } \
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CATEGORY_TYPE(category) virtual int GetCategoryFlags() const override { return category; }

    enum class EventType {
        None = 0,
        AppTick, AppUpdate, AppRender,
        WindowClosed, WindowResized, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication    = 1 << 0,
        EventCategoryInput          = 1 << 1,
        EventCategoryKeyboard       = 1 << 2,
        EventCategoryMouse          = 1 << 3,
        EventCategoryMouseButton    = 1 << 4
    };

    class Event {
        friend class EventDispatcher;

    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory e) {
            return GetCategoryFlags() & e;
        }

        bool Handled = false;
    };

    class EventDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& e) : m_Event(e) { }

        template<typename T>
        bool Dispatch(EventFn<T> func) {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.Handled = func(* (T*) &m_Event);
                return true;
            }

            return false;
        }

    protected:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    }

}

#endif //GRAPHICSTEMPLATE_EVENT_H
