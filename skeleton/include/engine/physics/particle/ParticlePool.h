#pragma once

#include <memory_resource>
#include <utility>
#include <vector>
#include <iostream>

#include "ObjectPool.h"

template<typename ParticleType>
class ParticlePool : public ObjectPool<ParticleType>
{
public:
    template<typename... Args>
    explicit ParticlePool(int maxParticles, Args&&... args)
        : ObjectPool<ParticleType>(maxParticles, std::forward<Args>(args)...)
    { }

    ~ParticlePool() = default;

    ParticleType* activateParticle() {
        ParticleType* p = ObjectPool<ParticleType>::activateObject();
        return p;
    }

    void deactivateParticle(int idx) {
        ObjectPool<ParticleType>::deactivateObject(idx);
    }

    std::pmr::vector<ParticleType*>& accessParticlePool() {
        return ObjectPool<ParticleType>::_objects;
    }
};