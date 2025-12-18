#pragma once

#include <memory_resource>
#include <utility>
#include <vector>
#include <iostream>

#include "ObjectPool.h"

template<typename RigidBodyType>
class RigidBodyPool : public ObjectPool<RigidBodyType>
{
public:
    template<typename... Args>
    explicit RigidBodyPool(int maxRigidBodies, Args&&... args)
        : ObjectPool<RigidBodyType>(maxRigidBodies, std::forward<Args>(args)...)
    { }

    ~RigidBodyPool() = default;

    RigidBodyType* activateRigidBody() {
        RigidBodyType* p = ObjectPool<RigidBodyType>::activateObject();
        return p;
    }

    void deactivateRigidBody(int idx) {
        ObjectPool<RigidBodyType>::deactivateObject(idx);
    }

    std::pmr::vector<RigidBodyType*>& accessRigidBodyPool() {
        return ObjectPool<RigidBodyType>::_objects;
    }
};