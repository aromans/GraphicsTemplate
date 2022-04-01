#ifndef GRAPHICSTEMPLATE_COMPONENT_H
#define GRAPHICSTEMPLATE_COMPONENT_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include "EcsTypes.h"

namespace Engine {

    class ComponentBase {
    public:
        virtual ~ComponentBase() {}

        virtual void DestroyData(unsigned char* data) const = 0;
        virtual void MoveData(unsigned char* src, unsigned char* dst) const = 0;
        virtual void CreateData(unsigned char* data) const = 0;

        virtual std::size_t GetSize() const = 0;
    };

    template<class C>
    class Component : public ComponentBase {
    public:
        virtual void DestroyData(unsigned char* data) const override {
            C* dataLoc = std::launder(reinterpret_cast<C*>(data));
            dataLoc->~C();
        }

        virtual void MoveData(unsigned char* src, unsigned char* dst) const override {
            new (&dst[0]) C(std::move(*reinterpret_cast<C*>(src)));
        }

        virtual void CreateData(unsigned char* data) const override {
            new (&data[0]) C();
        }

        virtual std::size_t GetSize() const override {
            return sizeof(C);
        }

        static ComponentTypeID GetTypeID() {
            return TypeIdGenerator<ComponentBase>::GetNewID<C>();
        }
    };

}


#endif //GRAPHICSTEMPLATE_COMPONENT_H
