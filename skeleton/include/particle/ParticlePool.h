#pragma once

#include <iostream>
#include <memory_resource>
#include <utility>

#include <vector>


template<typename ParticleType>
class ParticlePool 
{
private:
    std::vector<std::byte> _buffer;
    std::pmr::monotonic_buffer_resource _arena;
    std::pmr::vector<ParticleType*> _particles;
    int _activeCount = 0;
    int _maxParticles;

public:
    template<typename... Args>
    explicit ParticlePool(int maxParticles, Args&&... args)
        : _buffer(maxParticles * sizeof(ParticleType)),
          _arena(_buffer.data(), _buffer.size()),
          _particles(&_arena),
          _maxParticles(maxParticles)
    {
        _particles.reserve(maxParticles);

        for (int i = 0; i < maxParticles; ++i) {
            void* mem = _arena.allocate(sizeof(ParticleType), alignof(ParticleType));
            ParticleType* p = new (mem) ParticleType(std::forward<Args>(args)...);
            _particles.push_back(p);
        }
    }

    ~ParticlePool() noexcept {
        for (auto* p : _particles)
            p->~ParticleType();
    }

    ParticleType* activateParticle() {
        if (_activeCount >= _maxParticles) {
            std::cerr << "Pool full.\n";
            return nullptr;
        }
        ParticleType* p = _particles[_activeCount++];
        p->activate();
        return p;
    }

    void deactivate(int idx) {
        if (idx < 0 || idx >= _activeCount) return;
        _particles[idx]->deactivate();
        std::swap(_particles[idx], _particles[--_activeCount]);
    }

    std::pmr::vector<ParticleType*>& accessParticlePool() {
        return _particles;
    }

    int getActiveCount() const { return _activeCount; }
};