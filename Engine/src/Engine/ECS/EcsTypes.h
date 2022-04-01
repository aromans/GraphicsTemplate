#ifndef GRAPHICSTEMPLATE_ECSTYPES_H
#define GRAPHICSTEMPLATE_ECSTYPES_H

#include <cstdlib>
#include <vector>
#include <string>

namespace Engine {

    typedef std::int32_t IDType;
    typedef IDType EntityID;
    typedef IDType ComponentTypeID;
    typedef std::vector<ComponentTypeID> ArcheTypeID;
    typedef unsigned char* ComponentData;

    const IDType NULL_ENTITY = 0;

    struct Archetype {
        ArcheTypeID type;
        std::vector<ComponentData> componentData;
        std::vector<EntityID> entityIds;
        std::vector<std::size_t> componentDataSize;
    };

    template<class T>
    class TypeIdGenerator {
    private:
        static IDType m_Count;
    public:
        template<class U>
        static const IDType GetNewID() {
            static const IDType idCounter = m_Count++;
            return idCounter;
        }
    };

    template<class T> IDType TypeIdGenerator<T>::m_Count = 0;

}

#endif //GRAPHICSTEMPLATE_ECSTYPES_H
