#pragma once

#include <vector>
#include <memory>
#include <string>

#include "ParticleSystem.h"
#include "MeshData.h"

class ConstantParticleGenerator;
class ParticleWithMass;

class MeshSystem : public ParticleSystem
{
public:
    MeshSystem() = delete;
    MeshSystem(const std::string& filename);
    MeshSystem(const std::string& filename, float pointSize, double scale, const Vector4& color);

    ~MeshSystem() = default;

    virtual void init() override;
    virtual void update(double deltaTime) override;

protected:
	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
        return _meshData.vertices.size();
    }

    virtual void initParticleGeneratorAndPool();

    virtual void createParticlesAtMeshVertices();

    MeshData _meshData;

    float _pointSize;
    double _scale;
    Vector4 _color;

private:
    std::vector<
        std::pair<
            std::unique_ptr<ConstantParticleGenerator>,
            std::unique_ptr<ParticlePool<ParticleWithMass>>
            >
        > _generatorsAndPools;
};
