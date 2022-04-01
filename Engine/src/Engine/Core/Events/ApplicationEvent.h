#ifndef GRAPHICSTEMPLATE_APPLICATIONEVENT_H
#define GRAPHICSTEMPLATE_APPLICATIONEVENT_H

#include "Event.h"

#include <sstream>

namespace Engine {

    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() { }

        EVENT_CATEGORY_TYPE(EventCategoryApplication | EventCategoryInput);
        EVENT_CLASS_TYPE(EventType::WindowClosed);
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) { }

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizedEvent: " << m_Width << " x " << m_Height;
            return ss.str();
        }

        EVENT_CATEGORY_TYPE(EventCategoryApplication | EventCategoryInput);
        EVENT_CLASS_TYPE(EventType::WindowResized);
    private:
        unsigned int m_Width, m_Height;
    };

    class AppTickEvent : public Event {
    public:
        AppTickEvent() { }

        EVENT_CATEGORY_TYPE(EventCategoryApplication);
        EVENT_CLASS_TYPE(EventType::AppTick);
    };

    class AppUpdateEvent : public Event {
    public:
        AppUpdateEvent() { }

        EVENT_CATEGORY_TYPE(EventCategoryApplication);
        EVENT_CLASS_TYPE(EventType::AppUpdate);
    };

    class AppRenderEvent : public Event {
    public:
        AppRenderEvent() { }

        EVENT_CATEGORY_TYPE(EventCategoryApplication);
        EVENT_CLASS_TYPE(EventType::AppRender);
    };

}

#endif //GRAPHICSTEMPLATE_APPLICATIONEVENT_H
