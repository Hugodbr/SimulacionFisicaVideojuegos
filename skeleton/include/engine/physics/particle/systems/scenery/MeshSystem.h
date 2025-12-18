#pragma once

#include <vector>
#include <memory>
#include <string>

#include "ParticleSystem.h"
#include "ParticlePool.h"
#include "ParticleWithMass.h"
#include "MeshData.h"

class ConstantParticleGenerator;


class MeshSystem : public ParticleSystem
{
public:
    MeshSystem() = delete;
    MeshSystem(const std::string& filename);
    MeshSystem(const std::string& filename, float pointSize, double scale, const physx::PxVec4& color);

    ~MeshSystem() = default;

    virtual void init() override;
    virtual void update(double deltaTime) override;

    virtual void onRender(const glm::mat4& modelViewMat) override;

    virtual void setRenderableEntity(std::shared_ptr<Abs_Entity> renderable) override;
    virtual void render(const glm::mat4& modelViewMat) override;
    // virtual void load() override;
	// virtual void unload() override;

    virtual ParticlePool<ParticleWithMass>* getParticlePool() {
        // For now, only one generator // ! TODO
        return _generatorsAndPools[0].second.get();
    }

protected:
	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
        return _meshData.uniqueVertices.size();
    }

    virtual void initParticleGeneratorAndPool();

    virtual void createParticlesAtMeshVertices();

    virtual void applyForces() override;


protected:
    MeshData _meshData;

    float _pointSize;
    double _scale;
    physx::PxVec4 _color;

private:
    std::vector<
        std::pair<
            std::unique_ptr<ConstantParticleGenerator>,
            std::unique_ptr<ParticlePool<ParticleWithMass>>
            >
        > _generatorsAndPools;
};
