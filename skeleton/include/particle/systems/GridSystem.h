#pragma once

#include "ParticleSystem.h"
#include "ParticlePool.h"

class StaticParticle; 
class ConstantParticleGenerator;


class GridSystem : public ParticleSystem
{
public:
    GridSystem(const physx::PxBounds3 &region, float pointSize = 1.0f, double scale = 10.0, const Vector4& color = Constants::Color::White);
    ~GridSystem() = default;

    void init() override;
    void update(double deltaTime) override;

    void toggleVisibility();

protected:
	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
        return Constants::System::Grid::ReserveCountPerGenerator;
    }

    void initParticleGeneratorAndPool();

    void createParticlesInGrid();


protected:

    physx::PxBounds3 _region;
    float _pointSize;
    double _scale;
    Vector4 _color;

    bool _visible;

private:
    std::vector<
        std::pair<
            std::unique_ptr<ConstantParticleGenerator>,
            std::unique_ptr<ParticlePool<StaticParticle>>
            >
        > _generatorsAndPools;

};