#ifndef GRAPHICSTEMPLATE_ECS_H
#define GRAPHICSTEMPLATE_ECS_H

/* --- Reference --- */
/* https://indiegamedev.net/2020/05/19/an-entity-component-system-with-data-locality-in-cpp/ */

#include <vector>
#include <unordered_map>

#include "Engine/Core/Logger/Log.h"

#include "EcsTypes.h"
#include "Component.h"


namespace Engine {

    class SystemBase {
    public:
        virtual ~SystemBase() {}
        virtual ArcheTypeID GetKey() const = 0;
        virtual void DoAction(const float elapsedTime, Archetype* archetype) = 0;
    };

    class ECS {
    private:

        typedef std::unordered_map<ComponentTypeID,ComponentBase*>
                ComponentTypeIDBaseMap;

        struct Record
        {
            Archetype* archetype;
            std::size_t index;
        };

        typedef std::unordered_map<EntityID, Record> EntityArchetypeMap;

        typedef std::vector<Archetype*> ArchetypesArray;

        typedef std::unordered_map<std::uint8_t,std::vector<std::shared_ptr<SystemBase>>>
                SystemsArrayMap;

    public:

        ECS()
        :
                m_EntityIdCounter(1) { }

        ~ECS() {
            for (Archetype* archetype : m_Archetypes) {
                for (std::size_t i = 0; i < archetype->type.size(); ++i) {
                    const ComponentBase* const comp = m_ComponentMap[archetype->type[i]];
                    const std::size_t& dataSize = comp->GetSize();
                    for (std::size_t e = 0; e < archetype->entityIds.size(); ++e) {
                        comp->DestroyData(&archetype->componentData[i][e*dataSize]);
                    }

                    delete [] archetype->componentData[i];
                }

                delete archetype;
            }
            for (ComponentTypeIDBaseMap::value_type& p : m_ComponentMap) {
                delete p.second;
            }
        }

        EntityID GetNewID() {
            return m_EntityIdCounter++;
        }

        template<class C>
        void RegisterComponent() {
            ComponentTypeID componentTypeId = Component<C>::GetTypeID();

            if (m_ComponentMap.contains(componentTypeId))
                return;

            m_ComponentMap.emplace(componentTypeId, new Component<C>);
        }

        template<class C>
        bool IsComponentRegistered() {
            ComponentTypeID componentTypeId = Component<C>::GetTypeID();

            return m_ComponentMap.contains(componentTypeId);
        }

        void RegisterSystem(const std::uint8_t& layer, std::shared_ptr<SystemBase> system) {
            m_Systems[layer].push_back(system);
        }

        void RegisterEntity(const EntityID entityId) {
            Record dummyRecord;
            dummyRecord.archetype = nullptr;
            dummyRecord.index = 0;
            m_Entities[entityId] = dummyRecord;
        }

        void RunSystems(const std::uint8_t& layer, const float elapsedMilliseconds) {
            for(const std::shared_ptr<SystemBase>& system : m_Systems[layer])
            {
                const ArcheTypeID& key = system->GetKey();

                for(Archetype* archetype: m_Archetypes)
                {
                    if(std::includes(archetype->type.begin(), archetype->type.end(),
                                     key.begin(), key.end()))
                    {
                        system->DoAction(elapsedMilliseconds, archetype);
                    }
                }
            }
        }

        Archetype* GetArchetype(const ArcheTypeID& id) {
            for(Archetype* archetype : m_Archetypes)
            {
                if(archetype->type == id)
                    return archetype;
            }

            // archetype doesn't exist, so create a new one

            Archetype* newArchetype = new Archetype;
            newArchetype->type = id;
            m_Archetypes.push_back(newArchetype);

            // add an empty array for each component in the type
            for(ArcheTypeID::size_type i = 0; i < id.size(); ++i)
            {
                newArchetype->componentData.push_back(new unsigned char[0]);
                newArchetype->componentDataSize.push_back(0);
            }

            return newArchetype;
        }

        void PrepareCompData(ArcheTypeID newArchetypeId,
                             Archetype* newArchetype,
                             Archetype* oldArchetype,
                             const Record& record) {
            for (std::size_t j = 0; j < newArchetypeId.size(); ++j) {
                const ComponentTypeID& newCompId = newArchetypeId[j];

                const ComponentBase* const newComp = m_ComponentMap[newCompId];

                const std::size_t& newCompDataSize = newComp->GetSize();

                std::size_t currentSize = newArchetype->entityIds.size() * newCompDataSize;
                std::size_t newSize = currentSize + newCompDataSize;
                if (newSize > newArchetype->componentDataSize[j]) {
                    newArchetype->componentDataSize[j] *= 2;
                    newArchetype->componentDataSize[j] += newCompDataSize;
                    auto newData = new unsigned char[newSize];

                    for (std::size_t e = 0; e < newArchetype->entityIds.size(); ++e) {
                        newComp->MoveData(&newArchetype->componentData[j][e*newCompDataSize],
                                          &newData[e*newCompDataSize]);

                        newComp->DestroyData(&newArchetype->componentData[j][e*newCompDataSize]);
                    }

                    delete [] newArchetype->componentData[j];

                    newArchetype->componentData[j] = newData;
                }

                newComp->CreateData(&newArchetype->componentData[j][currentSize]);

                ArcheTypeID oldArchetypeId = oldArchetype->type;

                for (std::size_t i = 0; i < oldArchetype->type.size(); ++i) {
                    const ComponentTypeID& oldCompId = oldArchetype->type[i];

                    if (oldCompId == newCompId) {
                        const std::size_t& oldCompDataSize =
                                m_ComponentMap[oldCompId]->GetSize();

                        ComponentBase* removeWrapper = m_ComponentMap[oldCompId];
                        removeWrapper->MoveData(&oldArchetype->componentData[i][record.index*oldCompDataSize],
                                                &newArchetype->componentData[j][currentSize]);

                        removeWrapper->DestroyData(&oldArchetype->componentData[i][record.index*oldCompDataSize]);

                        break;
                    }
                }
            }
        }

        template<class C>
        void RemoveTargetComponent(Archetype* oldArchetype,
                                   ComponentTypeID compTypeId,
                                   const Record& record) {
            for (std::size_t i = 0; i < oldArchetype->type.size(); ++i) {
                const ComponentTypeID& oldCompTypeId = oldArchetype->type[i];

                if (oldCompTypeId == compTypeId) {
                    ComponentBase* removeWrapper = m_ComponentMap[compTypeId];
                    removeWrapper->DestroyData(&oldArchetype->componentData[i][record.index*sizeof(C)]);
                }

                const ComponentBase* const oldComp = m_ComponentMap[oldCompTypeId];

                const std::size_t& oldCompDataSize = oldComp->GetSize();

                std::size_t currentSize = oldArchetype->entityIds.size() * oldCompDataSize;
                std::size_t newSize = currentSize - oldCompDataSize;
                auto newData = new unsigned char[oldArchetype->componentDataSize[i] - oldCompDataSize];
                oldArchetype->componentDataSize[i] -= oldCompDataSize;
                for (std::size_t e = 0, ei = 0; e < oldArchetype->entityIds.size(); ++e) {
                    if (e == record.index)
                        continue;

                    oldComp->MoveData(&oldArchetype->componentData[i][e*oldCompDataSize],
                                      &newData[ei*oldCompDataSize]);
                    oldComp->DestroyData(&oldArchetype->componentData[i][e*oldCompDataSize]);

                    ++ei;
                }

                delete [] oldArchetype->componentData[i];

                oldArchetype->componentData[i] = newData;
            }
        }

        void AdjustEntityArchetypes(Archetype* oldArchetype,
                              EntityID entityId) {
            std::vector<EntityID>::iterator entitiesToRemove
                = std::find(oldArchetype->type.begin(),
                            oldArchetype->type.end(),
                            entityId);

            std::for_each(entitiesToRemove, oldArchetype->entityIds.end(),
                          [this, &oldArchetype](const EntityID& eid) {
                Record& moveRecord = m_Entities[eid];
                --moveRecord.index;
            });

            oldArchetype->entityIds.erase(
                    std::remove(oldArchetype->entityIds.begin(),
                                oldArchetype->entityIds.end(),
                                entityId), oldArchetype->entityIds.end());
        }

        template<class C, typename... Args>
        C* AddComponent(const EntityID& entityId, Args&&... args) {
            ComponentTypeID newCompTypeId = Component<C>::GetTypeID();

            if(!IsComponentRegistered<C>())
            {
                throw std::runtime_error("Component Not Registered!");
            }

            const std::size_t& compDataSize = m_ComponentMap[newCompTypeId]->GetSize();

            // this ensures the entity is added to dummy archetype if needed
            Record& record = m_Entities[entityId];
            Archetype* oldArchetype = record.archetype;

            C* newComponent = nullptr; // will be returned

            Archetype* newArchetype = nullptr;

            if(oldArchetype)
            {
                if(std::find(oldArchetype->type.begin(),
                             oldArchetype->type.end(),
                             newCompTypeId)
                   != oldArchetype->type.end())
                {
                    // this entity already contains this component
                    return nullptr;
                }

                ArcheTypeID newArchetypeId = oldArchetype->type;
                newArchetypeId.push_back(newCompTypeId);
                std::sort(newArchetypeId.begin(), newArchetypeId.end());

                newArchetype = GetArchetype(newArchetypeId);

                for(std::size_t j = 0; j < newArchetypeId.size(); ++j)
                {
                    const ComponentTypeID& newCompId = newArchetypeId[j];

                    const ComponentBase* const newComp = m_ComponentMap[newCompId];

                    const std::size_t& newCompDataSize = newComp->GetSize();

                    std::size_t currentSize = newArchetype->entityIds.size() * newCompDataSize;
                    std::size_t newSize = currentSize + newCompDataSize;
                    if(newSize > newArchetype->componentDataSize[j])
                    {
                        newArchetype->componentDataSize[j] *= 2;
                        newArchetype->componentDataSize[j] += newCompDataSize;
                        unsigned char* newData = new unsigned char[newArchetype->componentDataSize[j]];
                        for(std::size_t e = 0; e < newArchetype->entityIds.size(); ++e)
                        {
                            newComp->MoveData(&newArchetype->componentData[j][e*newCompDataSize],
                                              &newData[e*newCompDataSize]);
                            newComp->DestroyData(&newArchetype->componentData[j][e*newCompDataSize]);
                        }
                        delete [] newArchetype->componentData[j];

                        newArchetype->componentData[j] = newData;
                    }

                    ArcheTypeID oldArchetypeId = oldArchetype->type;

                    for(std::size_t i = 0; i < oldArchetype->type.size(); ++i)
                    {
                        const ComponentTypeID& oldCompId = oldArchetype->type[i];
                        if(oldCompId == newCompId)
                        {
                            const ComponentBase* const oldComp = m_ComponentMap[oldCompId];

                            const std::size_t& oldCompDataSize = oldComp->GetSize();

                            oldComp->MoveData(&oldArchetype->componentData[i][record.index*oldCompDataSize],
                                              &newArchetype->componentData[j][currentSize]);
                            oldComp->DestroyData(&oldArchetype->componentData[i][record.index*oldCompDataSize]);

                            goto cnt;
                        }
                    }

                    newComponent
                            = new (&newArchetype->componentData[j][currentSize])
                            C(std::forward<Args>(args)...);

                    cnt:;
                }

                if(!oldArchetype->entityIds.empty())
                {
                    for(std::size_t i = 0; i < oldArchetype->type.size(); ++i)
                    {
                        const ComponentTypeID& oldCompTypeID = oldArchetype->type[i];

                        if(oldCompTypeID == newCompTypeId)
                        {
                            ComponentBase* removeWrapper = m_ComponentMap[newCompTypeId];
                            removeWrapper->DestroyData(
                                    &oldArchetype->componentData[i][record.index*sizeof(C)]);
                        }

                        const ComponentBase* const oldComp = m_ComponentMap[oldCompTypeID];

                        const std::size_t& oldCompDataSize = oldComp->GetSize();

                        std::size_t currentSize = oldArchetype->entityIds.size() * oldCompDataSize;
                        std::size_t newSize = currentSize - oldCompDataSize;
                        unsigned char* newData = new unsigned char[oldArchetype->componentDataSize[i]-oldCompDataSize];
                        oldArchetype->componentDataSize[i] -= oldCompDataSize;
                        for(std::size_t e = 0, ei = 0; e < oldArchetype->entityIds.size(); ++e)
                        {
                            if(e == record.index)
                                continue;

                            oldComp->MoveData(&oldArchetype->componentData[i][e*oldCompDataSize],
                                              &newData[ei*oldCompDataSize]);
                            oldComp->DestroyData(&oldArchetype->componentData[i][e*oldCompDataSize]);

                            ++ei;
                        }

                        delete [] oldArchetype->componentData[i];

                        oldArchetype->componentData[i] = newData;
                    }
                }

                std::vector<EntityID>::iterator willBeRemoved
                        = std::find(oldArchetype->entityIds.begin(),
                                    oldArchetype->entityIds.end(),
                                    entityId);

                std::for_each(willBeRemoved, oldArchetype->entityIds.end(),
                              [this,&oldArchetype](const EntityID& eid)
                              {
                                  Record& moveR = m_Entities[eid];
                                  --moveR.index;
                              });

                oldArchetype->entityIds.erase(willBeRemoved);
            }
            else
            {
                ArcheTypeID newArchetypeId(1, newCompTypeId);

                const ComponentBase* const newComp = m_ComponentMap[newCompTypeId];

                newArchetype = GetArchetype(newArchetypeId);

                std::size_t currentSize = newArchetype->entityIds.size() * compDataSize;
                std::size_t newSize = currentSize + compDataSize;
                if(newSize > newArchetype->componentDataSize[0])
                {
                    newArchetype->componentDataSize[0] *= 2;
                    newArchetype->componentDataSize[0] += compDataSize;
                    unsigned char* newData = new unsigned char[newArchetype->componentDataSize[0]];
                    for(std::size_t e = 0; e < newArchetype->entityIds.size(); ++e)
                    {
                        newComp->MoveData(&newArchetype->componentData[0][e*compDataSize],
                                          &newData[e*compDataSize]);
                        newComp->DestroyData(&newArchetype->componentData[0][e*compDataSize]);
                    }
                    delete [] (newArchetype->componentData[0]);

                    newArchetype->componentData[0] = newData;
                }

                newComponent
                        = new (&newArchetype->componentData[0][currentSize])
                        C(std::forward<Args>(args)...);
            }

            newArchetype->entityIds.push_back(entityId);
            record.index = newArchetype->entityIds.size()-1;
            record.archetype = newArchetype;

            return newComponent;
        }

        template<class C>
        void RemoveComponent(const EntityID& entityId) {
            if (!IsComponentRegistered<C>()) {
                return;
            }

            ComponentTypeID compTypeId = Component<C>::GetTypeID();

            if (!m_Entities.contains(entityId)) {
                return;
            }

            Record& record = m_Entities[entityId];
            Archetype* oldArchetype = record.archetype;

            if (!oldArchetype) {
                return;
            }

            if (std::find(oldArchetype->type.begin(),
                          oldArchetype->type.end(),
                          compTypeId) == oldArchetype->type.end()) {
                return;
            }

            ArcheTypeID newArchetypeId = oldArchetype->type;
            newArchetypeId.erase(std::remove(newArchetypeId.begin(),
                                             newArchetypeId.end(),
                                             compTypeId),
                                 newArchetypeId.end());

            std::sort(newArchetypeId.begin(), newArchetypeId.end());

            Archetype* newArchetype = GetArchetype(newArchetypeId);

            PrepareCompData(newArchetypeId, newArchetype, oldArchetype, record);

            RemoveTargetComponent<C>(oldArchetype, compTypeId, record);

            AdjustEntityArchetypes(oldArchetype, entityId);

            newArchetype->entityIds.push_back(entityId);
            record.index = newArchetype->entityIds.size() - 1;
            record.archetype = newArchetype;
        }

    private:
        EntityArchetypeMap m_Entities;

        ArchetypesArray m_Archetypes;

        EntityID m_EntityIdCounter;

        SystemsArrayMap m_Systems;

        ComponentTypeIDBaseMap m_ComponentMap;
    };

    template<class... Ts>
    ArcheTypeID SortKeys(ArcheTypeID types) {
        ENG_CORE_INFO("SORTING THE KEY");
        std::sort(types.begin(), types.end());
        return types;
    }

    template<class... Cs>
    class System : public SystemBase
    {
    public:

        friend class ECS;

        typedef std::function<void(const float,const std::vector<EntityID>&,Cs*...)> Func;

        System(ECS& ecs, const std::uint8_t& layer);

        virtual ArcheTypeID GetKey() const override;

        void Action(Func func);

    protected:

        template<std::size_t Index1, typename T, typename... Ts>
        std::enable_if_t<Index1==sizeof...(Cs)>
        DoAction(const float elapsedMilliseconds,
                 const ArcheTypeID& archeTypeIds,
                 const std::vector<EntityID>& entityIDs,
                 T& t,
                 Ts... ts);

        template<std::size_t Index1, typename T, typename... Ts>
        std::enable_if_t<Index1 != sizeof...(Cs)>
        DoAction(const float elapsedMilliseconds,
                 const ArcheTypeID& archeTypeIds,
                 const std::vector<EntityID>& entityIDs,
                 T& t,
                 Ts... ts);

        virtual void DoAction(const float elapsedMilliseconds,
                              Archetype* archetype) override;

        ECS& m_ecs;
        Func m_func;
        bool m_funcSet;
    };

    template<class... Cs>
    void System<Cs...>::Action(Func func)
    {
        m_func = func;
        m_funcSet = true;
    }

    template<class... Cs>
    System<Cs...>::System(ECS& ecs, const std::uint8_t& layer)
            :
            m_ecs(ecs),
            m_funcSet(false)
    {
        m_ecs.RegisterSystem(layer, std::shared_ptr<SystemBase>(this));
    }

    template<class... Ts>
    ArcheTypeID sort_keys(ArcheTypeID types)
    {
        std::sort(types.begin(), types.end());
        return types;
    }

    template<class... Cs>
    ArcheTypeID System<Cs...>::GetKey() const
    {
        return sort_keys({{Component<Cs>::GetTypeID()...}});
    }

    template<class... Cs>
    void System<Cs...>::DoAction(const float elapsedMilliseconds, Archetype* archetype)
    {
        if(m_funcSet)
            DoAction<0>(elapsedMilliseconds,
                        archetype->type,
                        archetype->entityIds,
                        archetype->componentData);
    }

    template<class... Cs>
    template<std::size_t Index, typename T, typename... Ts>
    std::enable_if_t<Index != sizeof...(Cs)>
    System<Cs...>::DoAction(const float elapsedMilliseconds,
                            const ArcheTypeID& archeTypeIds,
                            const std::vector<EntityID>& entityIDs,
                            T& t,
                            Ts... ts)
    {
        using IthT = typename std::tuple_element<Index, std::tuple<Cs...>>::type;
        std::size_t index2 = 0;
        ComponentTypeID thisTypeCS = Component<IthT>::GetTypeID();
        ComponentTypeID thisArchetypeID = archeTypeIds[index2];
        while(thisTypeCS != thisArchetypeID && index2 < archeTypeIds.size())
        {
            ++index2;
            thisArchetypeID = archeTypeIds[index2];
        }
        if(index2 == archeTypeIds.size())
        {
            throw std::runtime_error
                    ("System was executed against an incorrect Archetype");
        }

        DoAction<Index+1>(elapsedMilliseconds,
                          archeTypeIds,
                          entityIDs,
                          t,
                          ts...,
                          reinterpret_cast<IthT*>(&t[index2][0]));
    }

    template<class... Cs>
    template<std::size_t Index, typename T, typename... Ts>
    std::enable_if_t<Index==sizeof...(Cs)>
    System<Cs...>::DoAction(const float elapsedMilliseconds,
                            const ArcheTypeID& archeTypeIds,
                            const std::vector<EntityID>& entityIDs,
                            T& t,
                            Ts... ts)
    {
        m_func(elapsedMilliseconds, entityIDs, ts...);
    }

}

#endif //GRAPHICSTEMPLATE_ECS_H
