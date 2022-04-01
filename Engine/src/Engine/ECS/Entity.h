#ifndef GRAPHICSTEMPLATE_ENTITY_H
#define GRAPHICSTEMPLATE_ENTITY_H

#include <cstdlib>
#include "EcsTypes.h"
#include "ECS.h"

namespace Engine {

    class Entity {
    public:
        explicit Entity(ECS& ecs) :
            m_Id(ecs.GetNewID()),
            m_Ecs(ecs) {
            m_Ecs.RegisterEntity(m_Id);
        }

        template<class C, typename... Args>
        C* Add(Args&&... args) {
            return m_Ecs.AddComponent<C>(m_Id, std::forward<Args>(args)...);
        }

        template<class C>
        C* Add(C&& c) {
            return m_Ecs.AddComponent<C>(m_Id, std::forward<C>(c));
        }

        EntityID GetID() const {
            return m_Id;
        }

    private:
        EntityID m_Id;
        ECS& m_Ecs;
    };

}

#endif //GRAPHICSTEMPLATE_ENTITY_H
