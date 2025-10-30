#pragma once

#include <iostream>
#include <memory_resource>
#include <utility>

#include <vector>


template<typename ParticleType>
class ParticlePool 
{
private:
    std::pmr::monotonic_buffer_resource m_Arena;
    std::pmr::vector<ParticleType*> m_Particles;
    int m_ActiveCount = 0;
    int m_MaxParticles;

public:
    template<typename... Args>
    explicit ParticlePool(int maxParticles, Args&&... args)
        : m_Arena(maxParticles * sizeof(ParticleType)),
          m_Particles(&m_Arena),
          m_MaxParticles(maxParticles)
    {
        m_Particles.reserve(maxParticles);

        // Construct all particles up-front
        for (int i = 0; i < maxParticles; ++i) {
            void* mem = m_Arena.allocate(sizeof(ParticleType), alignof(ParticleType));
            ParticleType* p = new (mem) ParticleType(std::forward<Args>(args)...);
            m_Particles.push_back(p);
        }
    }

    ~ParticlePool() {
        for (auto* p : m_Particles)
            p->~ParticleType();
    }

    ParticleType* activate() {
        if (m_ActiveCount >= m_MaxParticles) {
            std::cerr << "Pool full.\n";
            return nullptr;
        }
        ParticleType* p = m_Particles[m_ActiveCount++];
        p->activate();
        return p;
    }

    void deactivate(int idx) {
        if (idx < 0 || idx >= m_ActiveCount) return;
        m_Particles[idx]->deactivate();
        std::swap(m_Particles[idx], m_Particles[--m_ActiveCount]);
    }

    std::pmr::vector<ParticleType*>& particles() {
        return m_Particles;
    }

    int getActiveCount() const { return m_ActiveCount; }
};