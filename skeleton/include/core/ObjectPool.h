#pragma once

#include <memory_resource>
#include <utility>
#include <vector>
#include <iostream>


template<typename ObjectType>
class ObjectPool 
{
protected:
    uint64_t total_allocated_bytes = 0;
    uint64_t total_active_objects = 0;

    std::vector<std::byte> _buffer;
    std::pmr::monotonic_buffer_resource _arena;
    std::pmr::vector<ObjectType*> _objects;
    int _activeCount = 0;
    int _maxObjects;

public:
    template<typename... Args>
    explicit ObjectPool(int maxObjects, Args&&... args)
        : _buffer(maxObjects * sizeof(ObjectType)),
          _arena(_buffer.data(), _buffer.size()),
          _objects(&_arena),
          _maxObjects(maxObjects)
    {
        _objects.reserve(maxObjects);

        for (int i = 0; i < maxObjects; ++i) {
            void* mem = _arena.allocate(sizeof(ObjectType), alignof(ObjectType));
            ObjectType* obj = new (mem) ObjectType(std::forward<Args>(args)...);
            obj->init();
            _objects.push_back(obj);
            total_active_objects++;
        }
        total_allocated_bytes += maxObjects * sizeof(ObjectType);
        std::cout << "ObjectPool created with " << maxObjects << " objects. Total allocated bytes: " << total_allocated_bytes << "\n";
        std::cout << "Total active objects: " << total_active_objects << "\n";
    }

    virtual ~ObjectPool() noexcept {
        for (auto* obj : _objects)
            obj->~ObjectType();
    }

    virtual ObjectType* activateObject() {
        if (_activeCount >= _maxObjects) {
            std::cout << "Pool full.\n";
            return nullptr;
        }
        ObjectType* obj = _objects[_activeCount++];
        obj->activate();
        return obj;
    }

    virtual void deactivateObject(int idx) {
        if (idx < 0 || idx >= _activeCount) return;
        _objects[idx]->deactivate();
        std::swap(_objects[idx], _objects[--_activeCount]);
    }

    virtual std::pmr::vector<ObjectType*>& accessObjectPool() {
        return _objects;
    }

    virtual int getActiveCount() const { return _activeCount; }
};